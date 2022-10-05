let slider = document.getElementById('myRange');

function setSlider(value)
{
	let t = value;
	chart.animation.pause();
	chart.animation.seek(t / 10 + '%');
}

function initSlider(chart) {
	slider.addEventListener('input', (e) => { setSlider(e.target.value); });
	chart.on('update', (ev) => { slider.value = ev.data.progress * 1000; })
};
