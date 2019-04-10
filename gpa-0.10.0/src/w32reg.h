/* w32reg.h - MS-Windows Registry access
   Copyright (C) 1998 Free Software Foundation, Inc.
   Copyright (C) 2008 g10 Code GmbH.

   This file is part of GPA.

   GPA is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   GPA is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.  */

#ifndef W32REG_H
#define W32REG_H

/* Return a string from the Win32 Registry or NULL in case of error.
   Caller must release the return value with g_free ().  A NULL for
   root is an alias for HKEY_CURRENT_USER with a fallback for
   HKEY_LOCAL_MACHINE.  */
char *read_w32_registry_string (const char *root,
				const char *dir, const char *name);

/* Write string VALUE into the W32 Registry ROOT, DIR and NAME.  If
   NAME does not exist, it is created.  */
int write_w32_registry_string (const char *root, const char *dir, 
			       const char *name, const char *value);

#endif	/* W32REG_H */
