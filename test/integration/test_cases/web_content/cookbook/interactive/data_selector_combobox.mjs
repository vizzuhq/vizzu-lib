import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
	chart => {
		let select = document.createElement("select");
		select.style="position: absolute; top: 40px; left: 300px;";
		chart._container.parentElement.appendChild(select);

		let options = ['Value 2 (+)', 'Value 3 (+)', 'Value 1 (+)'];
		for (var i = 0; i < options.length; i++) {
			var option = document.createElement("option");
			option.value = options[i];
			option.text = options[i];
			select.appendChild(option);
		}

		select.addEventListener("change", (event) => {
			chart.animate({
				data: data,
				config: {
					y: event.target.value,
					label: event.target.value
				}
			})	
		});

		return chart.animate({
			data: data,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				label: 'Value 2 (+)'
			}
		},0)
	}
];

export default testSteps;