/*
 * gladeviewer: GTK+ UI File Viewer
 * Copyright (C) 2011 Eiichi Sato
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtkmm.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <sstream>

std::string usage() {
	return
		"Usage:\n"
		"  gladeviewer GLADE -w WIDGET [OPTIONS...] - GTK+ UI File Viewer\n"
		"\n"
		"Options:\n"
		"  -w, --widget\t\tName of a widget to preview.\n"
		"  -h, --help\t\tShow help options.\n"
		;
}

void abort() { exit(1); }
void abort(const std::string &msg) { std::cerr << msg << std::endl; abort(); }
void abort_with_usage() { std::cerr << usage() << std::endl; abort(); }
void abort_with_usage(const std::string &msg) { std::cerr << msg << "\n\n" << usage() << std::endl; abort(); }

int main(int argc, char **argv)
{
	Gtk::Main kit(argc, argv);

	static struct option long_options[] =
	{
		{"widget", required_argument, 0, 'w'},
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};

	const char *glade_file = NULL;
	const char *widget_name = NULL;

	while (1) {
		int option_index = 0;
		int c = getopt_long(argc, argv, "w:h", long_options, &option_index);
		if (c == -1)
			break;
		switch (c) {
			case 'w':
				widget_name = optarg;
				break;
			case 'h':
				abort_with_usage();
				break;
			default:
				std::cerr << std::endl;
				abort_with_usage();
				break;
		}
	}

	if (optind >= argc || !widget_name)
		abort_with_usage("Error: GLADE or WIDGET not specified.");
	glade_file = argv[optind];

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	try {
		builder->add_from_file(glade_file);
	} catch(const Glib::FileError& e) {
		abort(e.what());
	} catch(const Glib::MarkupError& e) {
		abort(e.what());
	} catch(const Gtk::BuilderError& e) {
		abort(e.what());
	}

	Gtk::Widget *widget = NULL;
	builder->get_widget(widget_name, widget);
	if (!widget)
		abort("Error: Cannot find a widget for specified name.");

	Gtk::Window *window = dynamic_cast<Gtk::Window*>(widget);
	if (window == NULL) {
		window = new Gtk::Window();
		if (widget->get_parent()) {
			widget->reparent(*window);
		} else {
			window->add(*widget);
		}
	}

	kit.run(*window);
	return 0;
}
