const data = {
	series: [
		{
			name: 'Genres',
			type: 'dimension',
			values: [
				'Pop', 'Rock', 'Jazz', 'Metal', 
				'Pop', 'Rock', 'Jazz', 'Metal', 
				'Pop', 'Rock', 'Jazz', 'Metal',
			]
		},
		{
			name: 'Types',
			type: 'dimension',
			values: [
				'Hard', 'Hard', 'Hard', 'Hard', 
				'Smooth', 'Smooth', 'Smooth', 'Smooth', 
				'Experimental', 'Experimental', 'Experimental', 'Experimental'
			]
		},
		{
			name: 'Popularity',
			type: 'measure',
			values: [
				114, 96, 78, 52, 56, 36, 174, 121, 127, 83, 94, 58
			]
		},
	]
};

export default data;
