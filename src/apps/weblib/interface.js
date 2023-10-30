mergeInto(LibraryManager.library, {
  openUrl: function (url) {
    window.open(UTF8ToString(url), '_blank')
  },
  callLater: function (func, obj, later) {
    setTimeout(() => Module.callback(func, obj), Math.max(later, 0))
  }
})
