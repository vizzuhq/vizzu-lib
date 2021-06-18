import Vizzu from './lib/vizzu.js';

function onLoaded()
{
	chart.addEventListener("vizzu.testEvents.xyParam", (param) => {
		chart.removeEventListener("vizzu.testEvents.xyParam");
		console.log("evnet: x = " + param.x + " y = " + param.y);
		param.x = 42;
		param.y = 24;
	});

	let data = {
			series: [
				{
					name: 'Colors',
					type: 'categories',
					values: ['red', 'green', 'blue']
				},
				{
					name: 'Val',
					type: 'values',
					values: [ 3, 5, 4 ]
				}
			]
		};

	chart.animate(
	{
		data: data,
		descriptor : {
			channels: {
				x: { attach: [ 'Colors'] },
			},
			title: null,
			legend: null,
		}
	}).then(() =>
		chart.animate({
			descriptor : {
				channels: {
					x: { detach: [ 'Colors'] },
					y: { attach: [ 'Colors' ]}
				},
			}
		})
	).then(() =>
		chart.animate({
			descriptor : {
				channels: {
					color: { attach: [ 'Colors' ]}
				}
			}
		})
	).then(() =>
		chart.animate({
			descriptor : {
				channels: {
					color: { detach: [ 'Colors' ]},
					lightness: { attach: [ 'Colors' ]}
				}
			}
		})
	).then(() =>
		chart.animate({
			descriptor : {
				channels: {
					lightness: { detach: [ 'Colors' ]},
					label: { attach: [ 'Colors' ]}
				}
			}
		})
	)
	.catch((err) =>
	{
		console.log(err);
	});
}

let slider = document.getElementById("myRange");
let chart = new Vizzu('vizzuCanvas', onLoaded);

slider.oninput = (e)=>
{
	let t = e.target.value;
	chart.animation.pause();
	chart.animation.seek(t/10 + '%');
};
