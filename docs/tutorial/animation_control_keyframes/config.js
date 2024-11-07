export default [
	[
		{
			name: '../assets/setup/setup_c',
			replace: [
				[
					'config: {',
					"config: {title: 'Jumping from 0% to 50% progress at the begining of the animation',"
				]
			]
		},
		{
			name: '01',
			returnOriginal: true
		}
	],
	[
		'02_a',
		{
			name: '02_b',
			returnOriginal: true
		}
	],
	[
		'03_a',
		{
			name: '03_b',
			returnOriginal: true
		}
	]
]
