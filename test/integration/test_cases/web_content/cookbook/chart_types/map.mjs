import { data } from '../../../../test_data/capitals.mjs';

const testSteps = [
	async chart => 
	{
		function urlToImage(url) {
			return new Promise(resolve => {
				const image = new Image();
				image.addEventListener('load', () => { resolve(image); });
				image.src = url;
			});
		}

		var map = await urlToImage('https://upload.wikimedia.org/wikipedia/commons/5/51/BlankMap-Equirectangular.svg');

		chart.on('plot-area-draw', event => {
			event.renderingContext.globalAlpha = 0.25; // the map image is too dark
			event.renderingContext.drawImage(map,
				event.data.rect.pos.x, event.data.rect.pos.y,
				event.data.rect.size.x, event.data.rect.size.y);
			event.renderingContext.globalAlpha = 1;
			event.preventDefault();
		});

		chart.feature('tooltip', true);

		return chart.animate({
			data: data,
			config: {
				title: 'Population of Capitals',
				x: { 
					set: 'Longitude', 
					range: { min: -180, max: +180 },
					axis: false,
					interlacing: false, 
					markerGuides: false,
					title: null 
				},
				y: { 
					set: 'Latitude', 
					range: { min: -90, max: +90 }, 
					axis: false,
					interlacing: false, 
					markerGuides: false,
					title: null 
				},
				noop: 'Capital',
				size: 'Population',
				geometry: 'circle'
			}
		})
	}
];

export default testSteps;