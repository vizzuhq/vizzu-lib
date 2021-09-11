import { data } from '../docs/content/tutorial/tutorial_data.js';
import Vizzu from './lib/vizzu.js';

let chart = new Vizzu('vizzuCanvas');

let labelDrawHandler;
let clickHandler;
let logoDrawHandler;

/*
We can register handlers for various events happening in the library.
There are mouse events (click, mouseon), animation events (begin, update, complete),
and rendering events called before rendering the chart elements.
Handlers can be registered/unregistered with the on(), off() method pair.
*/
chart.initializing.then(
	// Induló állapot
	chart => chart.animate({
		data: data,
		config: {
			channels: {
				y: { set: ['Popularity', 'Types'] },
				x: { attach: ['Genres'] },
				color: { attach: ['Types'] },
				label: { set: ['Popularity'] },
			},
			title: 'Style settings'
		}
	})
)

/** We are registering a handler for the click event which will 
 *  show an alert block with the meta information of the clicked marker.
 */
.then(chart => {
	clickHandler = event => {
		alert(JSON.stringify(event.data));
	};		
	chart.on('click', clickHandler);
	return chart.animate({
		config: {
			title: 'Click on marker.'
		}
	});
})

/** Unregistering the previously registered handler. */
.then(chart => {
	chart.off('click', clickHandler);
	return chart.animate({
		config: {
			title: 'Click event handler removed.'
		}
	});
})

/* Here we override the axis label color for 'Jazz' to red and all others to 'gray'. */
.then(chart => {
	labelDrawHandler = event => {
		event.renderingContext.fillStyle =
			(event.data.text === 'Jazz')
			? 'red' : 'gray';
	};
	chart.on('plot-axis-label-draw', labelDrawHandler);
	return chart.animate({
		config: {
			title: 'Changing the canvas context before label draw.'
		}
	});
})

/** Unregistering the previously registered handler. */
.then(chart => {
	chart.off('plot-axis-label-draw', labelDrawHandler);
	return chart.animate({
		config: {
			title: 'Restored default behavior.'
		}
	});
})

/* The default behaviour of all events can be blocked by calling the event's
preventDefault() method. For example we block the drawing of the Vizzu Logo. */
.then(chart => {
	logoDrawHandler = event => { event.preventDefault() };
	chart.on('logo-draw', logoDrawHandler);
	return chart.animate({
		config: {
			title: 'Prevent default behavior.'
		}
	});
})

/** Unregistering the previously registered handler. */
.then(chart => {
	chart.off('logo-draw', logoDrawHandler);
	return chart.animate({
		config: {
			title: 'Prevent default behavior.'
		}
	});
})

