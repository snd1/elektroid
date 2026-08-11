/*
 *   default.c
 *   Copyright (C) 2023 David García Goñi <dagargo@gmail.com>
 *
 *   This file is part of Elektroid.
 *
 *   Elektroid is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Elektroid is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Elektroid. If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib/gi18n.h>
#include "default.h"
#include "common.h"

#define DEFAULT_MAX_PROGRAMS 128

enum default_fs
{
  FS_PROGRAM_DEFAULT
};

static gint
default_read_dir (struct backend *backend, struct item_iterator *iter,
		  const gchar *dir, const gchar **extensions)
{
  struct common_simple_read_dir_data *data;

  if (strcmp (dir, "/"))
    {
      return -ENOTDIR;
    }

  data = g_malloc (sizeof (struct common_simple_read_dir_data));
  data->next = 0;
  data->last = DEFAULT_MAX_PROGRAMS - 1;

  item_iterator_init (iter, dir, data, common_simple_next_dentry, g_free);

  return 0;
}

const struct fs_operations FS_PROGRAM_DEFAULT_OPERATIONS = {
  .id = FS_PROGRAM_DEFAULT,
  .options = FS_OPTION_SINGLE_OP | FS_OPTION_SLOT_STORAGE |
    FS_OPTION_SHOW_ID_COLUMN,
  .name = "program",
  .gui_name = "Programs",
  .gui_icon = FS_ICON_PRESET,
  .file_icon = FS_ICON_FILE,
  .readdir = default_read_dir,
  .select_item = common_midi_program_change,
  .get_exts = common_get_all_extensions
};

static gint
default_handshake (struct backend *backend)
{
  gslist_fill (&backend->fs_ops, &FS_PROGRAM_DEFAULT_OPERATIONS, NULL);
  snprintf (backend->name, LABEL_MAX, "%s", _("MIDI device"));
  return 0;
}

const struct connector CONNECTOR_DEFAULT = {
  .name = "default",
  .handshake = default_handshake,
  .type = CONNECTOR_TYPE_MIDI
};
