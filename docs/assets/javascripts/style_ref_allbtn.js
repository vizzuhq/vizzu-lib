const pAll = document.getElementById('allbtn-style')
const btnAll = pAll.getElementsByTagName('button')[0]
pAll.addEventListener('click', function () {
	const collapsible = btnAll.textContent.includes('-')
	const coll = document.getElementsByClassName('collapsible-style')
	for (let i = 0; i < coll.length; i++) {
		const button = coll[i].getElementsByTagName('button')[0]
		if (collapsible && button.textContent.includes('-')) {
			coll[i].click()
			btnAll.textContent = '+ expand all'
		}
		if (!collapsible && button.textContent.includes('+')) {
			coll[i].click()
			btnAll.textContent = '- collapse all'
		}
	}
})
