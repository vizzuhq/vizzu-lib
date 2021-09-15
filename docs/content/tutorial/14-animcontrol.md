## Animation control

> With these options, you can play, pause, stop, seek or reverse the animations.

### Jumping from 75% to 25% progress during the animation

> In this step, we set an event that pauses the animation when it reaches 75% of 
> progress and seeks back to 25% of progress, then restarts the animation from 
> there.
> 
> ```javascript
> function onUpdate(event) {
> 	if (event.data.progress > 0.75) {
> 		chart.animation.pause();
> 		chart.off('update', onUpdate);
> 		setTimeout(() => {
> 			chart.animation.seek('25%');
> 			chart.animation.play();
> 		}, 1000);
> 	}
> };
> 
> chart.on('update', onUpdate);
> 
> chart.animate({
> 	config: {
> 		channels: {
> 			x: { attach: ['Types'] },
> 			y: { detach: ['Types'] }
> 		}
> 	}
> })
> ```