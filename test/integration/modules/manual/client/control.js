export function initSlider(chart) {
	let slider = document.getElementById('myRange');
	slider.oninput = (e) => {
	  let t = e.target.value;
	  chart.animation.pause();
	  chart.animation.seek(t / 10 + '%');
	};
	chart.on('update', (ev) => {
	  slider.value = ev.data.progress * 1000;
	})
  };
  