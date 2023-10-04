function changeTarget(links, target) {
  for (let i = 0; i < links.length; i++) {
    if (links[i].hostname !== window.location.hostname || links[i].href.includes('/assets/')) {
      links[i].target = target
    }
  }
}

document.addEventListener('DOMContentLoaded', (event) => {
  const target = '_blank'
  changeTarget(document.links, target)

  const iframe = document.getElementById('coviframe')
  if (iframe) {
    iframe.addEventListener('load', (event) => {
      changeTarget(iframe.contentWindow.document.getElementsByTagName('a'), target)
    })
  }
})
