/*
 * 1394-Based Digital Camera Control Library
 *
 * Mac OS X Digital Camera Capture Code
 *
 * Written by David Moore <dcm@acm.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


/*
 // moved to dc1394_capture.h
 
#ifndef __DC1394_CAPTURE_MACOSX_H__
#define __DC1394_CAPTURE_MACOSX_H__

typedef void (*dc1394capture_callback_t)(dc1394camera_t *, void *);

int
dc1394_capture_schedule_with_runloop (dc1394camera_t * camera,
        CFRunLoopRef run_loop, CFStringRef run_loop_mode);
void
dc1394_capture_set_callback (dc1394camera_t * camera,
        dc1394capture_callback_t callback, void * user_data);

#endif
*/