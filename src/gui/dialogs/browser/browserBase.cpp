/* -----------------------------------------------------------------------------
 *
 * Giada - Your Hardcore Loopmachine
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2017 Giovanni A. Zuliani | Monocasual
 *
 * This file is part of Giada - Your Hardcore Loopmachine.
 *
 * Giada - Your Hardcore Loopmachine is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Giada - Your Hardcore Loopmachine is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Giada - Your Hardcore Loopmachine. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------- */


#include "../../../core/graphics.h"
#include "../../../core/conf.h"
#include "../../../core/const.h"
#include "../../../utils/gui.h"
#include "../../../utils/fs.h"
#include "../../elems/browser.h"
#include "../../elems/basics/button.h"
#include "../../elems/basics/input.h"
#include "../../elems/basics/progress.h"
#include "../../elems/basics/check.h"
#include "browserBase.h"


using std::string;
using namespace giada::m;


gdBrowserBase::gdBrowserBase(int x, int y, int w, int h, const string& title,
		const string& path, void (*callback)(void*))
	:	gdWindow(x, y, w, h, title.c_str()), callback(callback)
{
	set_non_modal();

	groupTop = new Fl_Group(8, 8, w-16, 40);
    hiddenFiles = new geCheck(groupTop->x(), groupTop->y(), 400, 20, "Show hidden files");
		where = new geInput(groupTop->x(), hiddenFiles->y()+hiddenFiles->h(), 20, 20);
		updir	= new geButton(groupTop->x()+groupTop->w()-20, where->y(), 20, 20, "", updirOff_xpm, updirOn_xpm);
	groupTop->end();
	groupTop->resizable(where);

  hiddenFiles->callback(cb_toggleHiddenFiles, (void*) this);

	where->readonly(true);
	where->cursor_color(G_COLOR_BLACK);
	where->value(path.c_str());

	updir->callback(cb_up, (void*) this);

	browser = new geBrowser(8, groupTop->y()+groupTop->h()+8, w-16, h-93);
	browser->loadDir(path);
	if (path == conf::browserLastPath)
		browser->preselect(conf::browserPosition, conf::browserLastValue);

	Fl_Group *groupButtons = new Fl_Group(8, browser->y()+browser->h()+8, w-16, 20);
		ok  	  = new geButton(w-88, groupButtons->y(), 80, 20);
		cancel  = new geButton(w-ok->w()-96, groupButtons->y(), 80, 20, "Cancel");
		status  = new geProgress(8, groupButtons->y(), cancel->x()-16, 20);
		status->minimum(0);
		status->maximum(1);
		status->hide();   // show the bar only if necessary
	groupButtons->resizable(status);
	groupButtons->end();

	end();

	cancel->callback(cb_close, (void*) this);

	resizable(browser);
	size_range(320, 200);

	gu_setFavicon(this);
	show();
}


/* -------------------------------------------------------------------------- */


gdBrowserBase::~gdBrowserBase()
{
	conf::browserX = x();
	conf::browserY = y();
	conf::browserW = w();
	conf::browserH = h();
	conf::browserPosition = browser->position();
	conf::browserLastPath = gu_dirname(browser->getSelectedItem());
	conf::browserLastValue = browser->value();
}


/* -------------------------------------------------------------------------- */


void gdBrowserBase::cb_up   (Fl_Widget* v, void* p) { ((gdBrowserBase*)p)->cb_up(); }
void gdBrowserBase::cb_close(Fl_Widget* v, void* p) { ((gdBrowserBase*)p)->cb_close(); }
void gdBrowserBase::cb_toggleHiddenFiles(Fl_Widget *v, void *p) { ((gdBrowserBase*)p)->cb_toggleHiddenFiles(); }


/* -------------------------------------------------------------------------- */


void gdBrowserBase::cb_up()
{
	string dir = browser->getCurrentDir();

	/* Take 'dir' path and remove all chars up to the next slash, e.g.:
		/path/to/my/dir -> /path/to/my
	Make sure not to remove the leading '/' (OS X/Linux only). */

	dir = dir.substr(0, dir.find_last_of(G_SLASH_STR));

#if defined(G_OS_MAC) || defined(G_OS_LINUX)

	if (dir.empty())
		dir = G_SLASH_STR;

#endif

	browser->loadDir(dir);
	where->value(browser->getCurrentDir().c_str());
}


/* -------------------------------------------------------------------------- */


void gdBrowserBase::cb_close()
{
	do_callback();
}


/* -------------------------------------------------------------------------- */


void gdBrowserBase::cb_toggleHiddenFiles()
{
	browser->toggleHiddenFiles();
}


/* -------------------------------------------------------------------------- */


void gdBrowserBase::setStatusBar(float v)
{
	status->value(status->value() + v);
	Fl::wait(0);
}


/* -------------------------------------------------------------------------- */


void gdBrowserBase::showStatusBar()
{
  status->show();
}


/* -------------------------------------------------------------------------- */


void gdBrowserBase::hideStatusBar()
{
  status->hide();
}


/* -------------------------------------------------------------------------- */


string gdBrowserBase::getCurrentPath()
{
  return where->value();
}


/* -------------------------------------------------------------------------- */


string gdBrowserBase::getSelectedItem()
{
	return browser->getSelectedItem();
}
