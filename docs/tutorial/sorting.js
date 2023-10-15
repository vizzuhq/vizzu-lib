const dataLoaded = import('../assets/data/music_data.js')
const mdChartLoaded = import('../assets/javascripts/mdchart.js')

Promise.all([dataLoaded, mdChartLoaded]).then((results) => {
  const data = results[0].default
  const MdChart = results[1].default
  const mdchart = new MdChart(data, 'tutorial')

  mdchart.create([
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: 'Switch to ascending order...',
              channels: {
                y: { set: ['Popularity', 'Kinds'] },
                x: { set: 'Genres' },
                color: { set: 'Kinds' },
                label: { set: 'Popularity' }
              }
            }
          })
        },
        (chart) => {
          return chart.animate({
            config: {
              sort: 'byValue'
            }
          })
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: '...or descending order.'
            }
          })
        },
        (chart) => {
          return chart.animate({
            config: {
              reverse: true
            }
          })
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: "Let's get back to where we were"
            }
          })
        },
        (chart) => {
          return chart.animate({
            config: {
              sort: 'none',
              reverse: false
            }
          })
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: 'With two discretes on one axis...'
            }
          })
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: {
                  detach: 'Kinds'
                },
                x: {
                  set: ['Genres', 'Kinds']
                }
              }
            }
          })
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: '...grouping is determined by their order.'
            }
          })
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                x: {
                  set: ['Kinds', 'Genres']
                }
              }
            }
          })
        }
      ]
    }
  ])
})
