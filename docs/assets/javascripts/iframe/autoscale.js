const parent = window.frameElement
if (parent) {
  window.addEventListener('resize', () => {
    const zoomElement = document.getElementById('zoom')
    let availableWidth = parent.clientWidth
    if (parent.clientWidth / parent.clientHeight > 1280 / 720) {
      availableWidth = (1280 * parent.clientHeight) / 720
    }
    const scale = availableWidth / 1280
    const translateY = (100 * (scale - 1)) / 2
    const translateX = (100 * (parent.clientWidth / 1280 - 1)) / 2
    const transform = `translate(${translateX}%, ${translateY}%) scale(${scale - 0.01})`
    zoomElement.style.transform = transform
    zoomElement.style.height = `${720 * scale}px`
    if (window.autoAdjustHeight) {
      window.autoAdjustHeight()
    }
  })
}
