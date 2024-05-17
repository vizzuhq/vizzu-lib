const collection = document.getElementsByClassName('showcase')
if (collection) {
	collection[0].addEventListener('click', () => {
		const iframe = document.getElementById('showcase')
		iframe.src = iframe.src // eslint-disable-line no-self-assign
	})
}
