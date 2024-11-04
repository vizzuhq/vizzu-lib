export default [
	[
		{
			name: '../assets/setup/setup_c',
			replace: [['config: {', "config: {title: 'Color palette',"]]
		},
		'01_a',
		{ name: '01_b', returnOriginal: true }
	],
	['02_a', '02_b'],
	['03_a', '03_b'],
	['04_a', '04_b'],
	['05_a', '05_b']
]
