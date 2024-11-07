const animationStore = {
	name: '03_b',
	replace: [
		['var animation\n\n', ''],
		['.activated.then((control) => {\n\t\tanimation = control.store()\n\t})', '']
	]
}

export default [
	[
		{
			name: '../assets/setup/setup_c',
			replace: [['config: {', "config: {title: 'When only the config property is used',"]]
		},
		{
			name: '01'
		}
	],
	['02'],
	['03_a', animationStore],
	['04_a', '04_b'],
	['05_a', '05_b'],
	['06_a', '06_b'],
	['07_a', '07_b'],
	['08', '04_a', animationStore],
	['09']
]
