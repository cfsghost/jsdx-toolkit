{
	'conditions': [
		['OS=="linux"', {
			'variables': {
				'with_clutter_gst': 'true',
				'with_widget': 'true',
				'with_x11': 'true'
			}
		}]
	],
	'targets': [
		{
			'target_name': 'jsdx_toolkit',
			'sources': [
				'src/jsdx_toolkit.cpp',
				'src/application.cpp',
				'src/window.cpp',
				'src/actor.cpp',
				'src/media.cpp',
				'src/container.cpp',
				'src/group.cpp',
				'src/stage.cpp',
				'src/rectangle.cpp',
				'src/text.cpp',
				'src/texture.cpp',
				'src/state.cpp'
			],
			'conditions': [
				['OS=="linux"', {
					'cflags': [
						'<!@(pkg-config --cflags clutter-1.0)',
						'-ansi',
						'-Wall'
					],
					'ldflags': [
						'<!@(pkg-config  --libs-only-L --libs-only-other clutter-1.0)'
					],
					'libraries': [
						'<!@(pkg-config  --libs-only-l --libs-only-other clutter-1.0)'
					],
					'conditions': [
						['with_clutter_gst=="true"', {
							'sources': [
								'src/gst/gst.cpp',
								'src/gst/video-texture.cpp'
							],
							'cflags': [
								'<!@(pkg-config --cflags clutter-gst-1.0)'
							],
							'ldflags': [
								'<!@(pkg-config  --libs-only-L --libs-only-other clutter-gst-1.0)'
							],
							'libraries': [
								'<!@(pkg-config  --libs-only-l --libs-only-other clutter-gst-1.0)'
							],
							'defines': [
								'ENABLE_CLUTTER_GST'
							]
						}],
						['with_widget=="true"', {
							'sources': [
								'src/widgets/style.cpp',
								'src/widgets/widget.cpp',
								'src/widgets/bin.cpp',
								'src/widgets/box_layout.cpp',
								'src/widgets/stack.cpp',
								'src/widgets/grid.cpp',
								'src/widgets/table.cpp',
								'src/widgets/adjustment.cpp',
								'src/widgets/scrollable.cpp',
								'src/widgets/button.cpp',
								'src/widgets/entry.cpp',
								'src/widgets/frame.cpp',
								'src/widgets/label.cpp',
								'src/widgets/dialog.cpp',
								'src/widgets/progress_bar.cpp',
								'src/widgets/slider.cpp',
								'src/widgets/toggle.cpp',
								'src/widgets/spinner.cpp',
								'src/widgets/image.cpp',
								'src/widgets/viewport.cpp',
								'src/widgets/scrollview.cpp',
								'src/widgets/kinetic_scrollview.cpp',
								'src/widgets/stylable.cpp'
							],
							'cflags': [
								'<!@(pkg-config --cflags mx-1.0)'
							],
							'ldflags': [
								'<!@(pkg-config  --libs-only-L --libs-only-other mx-1.0)'
							],
							'libraries': [
								'<!@(pkg-config  --libs-only-l --libs-only-other mx-1.0)'
							],
							'defines': [
								'ENABLE_MX',
								'ENABLE_WIDGET'
							]
						}],
						['with_x11=="true"', {
							'sources': [
								'src/backend/x11.cpp'
							],
							'cflags': [
								'<!@(pkg-config --cflags x11)'
							],
							'ldflags': [
								'<!@(pkg-config  --libs-only-L --libs-only-other x11)'
							],
							'libraries': [
								'<!@(pkg-config  --libs-only-l --libs-only-other x11)'
							],
							'defines': [
								'USE_X11'
							]
						}]
					]
				}]
			]
		}
	]
}
