chart.animate({
	data: {
		filter: (record) =>
			(record.Genres === 'Pop' || record.Genres === 'Metal') && record.Kinds === 'Smooth'
	}
})
