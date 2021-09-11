import tutorial from './tutorial-document.js';

tutorial.section('Animation control')
.p(`
With these options, you can play, pause, stop, seek or reverse the animations.
`)
.code('Animation control', chart => 
	chart.animate({
		config: {
			channels: {
				y: { set: ['Popularity', 'Types'] },
				x: { attach: ['Genres'] },
				color: { set: ['Types'] },
				label: { attach: ['Popularity'] }
			}
		}
	})
)
.h() 
.p(`
In this step, we set an event that pauses the animation when it reaches 75% of 
progress and seeks back to 25% of progress, then restarts the animation from 
there.
`)
.code('Jumping from 75% to 25% progress during the animation', chart => {
	chart.on('update', (event) => {
		if (event.data.progress > 0.75) {
			chart.animation.pause();
			chart.off('update');
			setTimeout(() => {
				chart.animation.seek('25%');
				chart.animation.play();
			}, 1000);
		}
	});
	return chart.animate({
		config: {
			channels: {
				x: { attach: ['Types'] },
				y: { detach: ['Types'] }
			}
		}
	})
})
