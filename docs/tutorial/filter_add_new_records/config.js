export default [
	[
		{
			name: '../assets/setup/setup_c',
			replace: [['config: {', "config: {title: 'Filter by one dimension',"]]
		},
		'01'
	],
	[
		{
			name: '02_a',
			initDataFilter: (record) => record.Genres === 'Pop' || record.Genres === 'Metal'
		},
		'02_b'
	],
	[
		{
			name: '03_a',
			initDataFilter: (record) =>
				(record.Genres === 'Pop' || record.Genres === 'Metal') && record.Kinds === 'Smooth'
		},
		'03_b'
	],
	[
		{
			name: '04_a',
			replace: [['config: {', 'data, config: {']],
			initDataFilter: (record) => record.Genres !== 'Soul'
		},
		{
			name: '04_b',
			replace: [['data: {', 'data: { filter: null,']]
		}
	]
]
