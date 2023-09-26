function calcMaxHeight(elem) {
  const content = elem.nextElementSibling
  const button = elem.getElementsByTagName('button')[0]
  if (button.textContent.includes('-')) {
    content.style.maxHeight = content.scrollHeight + 'px'
  } else {
    content.style.maxHeight = '0'
  }
  const nextParentContent = elem.closest('.collapsible-style-content')
  if (nextParentContent !== null) {
    const nextParentCollapsible = nextParentContent.previousElementSibling
    setTimeout(function () {
      calcMaxHeight(nextParentCollapsible)
    }, animationDelay)
  }
}

const coll = document.getElementsByClassName('collapsible-style')
const animationDelay = 110
for (let i = 0; i < coll.length; i++) {
  coll[i].addEventListener('click', function () {
    const button = this.getElementsByTagName('button')[0]
    if (button.textContent.includes('+')) {
      button.textContent = button.textContent.replace('+', '-')
    } else {
      button.textContent = button.textContent.replace('-', '+')
    }
    calcMaxHeight(this)
  })
}
