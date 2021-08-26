import Vizzu from 'https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js';
import {data} from '/data/trump_data.js';

const chart = new Vizzu('testVizzuCanvas');

let rightBarValue = 'Tweet type';
let anim = null;

function onRightBarChange(event)
{
	let oldValue = rightBarValue;
	let newValue = event.target.value;
	if (newValue !== oldValue)
	{
		rightBarValue = newValue;
		anim = anim.then(chart =>
			chart.animate({
				descriptor: {
					channels: {
						y: {detach: [oldValue], attach: [newValue]},
						color: {detach: [oldValue], attach: [newValue]},
					}
				}
			})
		);
	}
}

function onButtonClick(event)
{
	anim = anim.then(chart =>
		chart.animate({
			descriptor: {
				channels: {
					y: {attach: [event.target.value]}
				}
			}
		})
	);
}

function setup()
{
	let rad = document.rightBar.menu;
	for (var i = 0; i < rad.length; i++)
		rad[i].addEventListener('change', onRightBarChange);

	document.getElementById('btnTweets')
		.addEventListener('click', onButtonClick);
	document.getElementById('btnShared')
		.addEventListener('click', onButtonClick);
}

anim = chart.initializing.then((chart) =>
{
	setup();

	return chart.animate({
		data: data,
		descriptor: {
			channels: {
				x: {attach: ['year', 'month']},
				y: {attach: ['tweet count', 'Tweet type']},
				color: {attach: ['Tweet type']},
			},
			align: 'center',
			legend: 'color',
		}
	});
});

