export default [
	[
		{
			name: '../assets/setup/setup_c',
			replace: [['config: {', "config: {title: 'Filter by one dimension',"]]
		},
		'01'
	],
	{
		assets: {
			initDataFilter: (record) => record.Genres === 'Pop' || record.Genres === 'Metal'
		},
		anims: ['02_a', '02_b']
	},
	{
		assets: {
			initDataFilter: (record) =>
				(record.Genres === 'Pop' || record.Genres === 'Metal') && record.Kinds === 'Smooth'
		},
		anims: ['03_a', '03_b']
	},
	{
		assets: { initDataFilter: (record) => record.Genres !== 'Soul' },
		anims: [
			{
				name: '04_a',
				replace: [['config: {', 'data, config: {']]
			},
			{
				name: '04_b',
				replace: [['data: {', 'data: { filter: null,']]
			}
		]
	}
]
