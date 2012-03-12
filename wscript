import Options
from os import unlink, symlink, popen
from os.path import exists

srcdir = "."
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
	opt.tool_options("compiler_cxx")
	opt.add_option('--enable-clutter-gst', action='store', default=1, help='Enable clutter-gst to support multimedia [Default: True]')
	opt.add_option('--enable-widget', action='store', default=1, help='Enable widget support [Default: True]')

def configure(conf):
	conf.check_tool("compiler_cxx")
	conf.check_tool("node_addon")
	conf.check_cfg(package='clutter-1.0', uselib_store='CLUTTER', args='--cflags --libs')

	if Options.options.enable_clutter_gst:
		print "Enabled clutter-gst"
		conf.env["ENABLE_CLUTTER_GST"] = True
		conf.check_cfg(package='clutter-gst-1.0', uselib_store='CLUTTER_GST', args='--cflags --libs')

	if Options.options.enable_widget:
		print "Enabled Widget Support"
		conf.env["ENABLE_MX"] = True
		conf.env["ENABLE_WIDGET"] = True
		conf.check_cfg(package='mx-1.0', uselib_store='MX', args='--cflags --libs')

def build(bld):
	obj = bld.new_task_gen("cxx", "shlib", "node_addon")
	obj.cxxflags = ["-Wall", "-ansi", "-pedantic"]
	obj.target = "clutter"
	obj.source = """
		src/application.cpp
		src/window.cpp
		src/clutter.cpp
		src/actor.cpp
		src/media.cpp
		src/container.cpp
		src/group.cpp
		src/stage.cpp
		src/rectangle.cpp
		src/text.cpp
		src/texture.cpp
		src/state.cpp
		"""
	obj.cxxflags = ["-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE"]
	obj.uselib = "CLUTTER"

	if bld.env["ENABLE_CLUTTER_GST"]:
		obj.cxxflags.append("-DENABLE_CLUTTER_GST");
		obj.source += """
			src/gst/gst.cpp
			src/gst/video-texture.cpp
			"""

		obj.uselib += " CLUTTER_GST"

	if bld.env["ENABLE_MX"]:
		obj.cxxflags.append("-DENABLE_MX");

	if bld.env["ENABLE_WIDGET"]:
		obj.cxxflags.append("-DENABLE_WIDGET");
		obj.source += """
			src/widgets/style.cpp
			src/widgets/widget.cpp
			src/widgets/bin.cpp
			src/widgets/box_layout.cpp
			src/widgets/grid.cpp
			src/widgets/table.cpp
			src/widgets/adjustment.cpp
			src/widgets/scrollable.cpp
			src/widgets/button.cpp
			src/widgets/frame.cpp
			src/widgets/label.cpp
			src/widgets/progress_bar.cpp
			src/widgets/slider.cpp
			src/widgets/toggle.cpp
			src/widgets/spinner.cpp
			src/widgets/image.cpp
			src/widgets/viewport.cpp
			src/widgets/scrollview.cpp
			src/widgets/kinetic_scrollview.cpp
			src/widgets/FlickView.cpp
			"""

		obj.uselib += " MX"

def shutdown():
	if Options.commands['clean']:
		if exists('clutter.node'): unlink('clutter.node')
	else:
		if exists('build/default/clutter.node') and not exists('clutter.node'):
			symlink('build/default/clutter.node', 'clutter.node')
