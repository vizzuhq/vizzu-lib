export var data = {
	series: [
		{
			name: 'Parents',
			type: 'dimension',
			values: [
				'A', 'A', 'A', 'A', 'B', 'B', 'B', 'B', 'C', 'C', 'C', 'C', 'D', 'D', 'D', 'D'
			]
		},
		{
			name: 'Childs',
			type: 'dimension',
			values: [
				'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'
			]
		},
		{
			name: 'GrandChild',
			type: 'dimension',
			values: [
				'a', 'a', 'b', 'b', 'a', 'a', 'b', 'b', 'c', 'c', 'd', 'd', 'c', 'c', 'd', 'd'
			]
		},
		{
			name: 'Values parent',
			type: 'measure',
			values: [
				1, 2, 4, 3, 3, 4, 2, 1, 4, 3, 1, 2, 2, 1, 3, 4
			]
		},
		{
			name: 'Values child',
			type: 'measure',
			values: [
				0, -1, 5, 6, 6, 5, -1, 0, 5, 6, 0, -1, -1, 0, 6, 5
			]
		}
	]
};


