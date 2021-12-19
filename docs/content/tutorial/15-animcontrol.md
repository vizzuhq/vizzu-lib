## Animation control

Using the result object provided by the chart's animate method you can play, pause, stop, seek or reverse the animations.

In this step, we set an event that pauses the animation when it reaches 75% of 
progress and seeks back to 25% of progress, then restarts the animation from 
there.

```javascript { "title": "Jumping from 75% to 25% progress during the animation" }
let animation = chart.animate({
	config: {
		channels: {
			x: { attach: ['Types'] },
			y: { detach: ['Types'] }
		}
	}
});

function onUpdate(event) {
	if (event.data.progress > 0.75) {
		animation.pause();
		chart.off('update', onUpdate);
		setTimeout(() => {
			animation.seek('25%').play();
		}, 1000);
	}
};

chart.on('update', onUpdate);
```
