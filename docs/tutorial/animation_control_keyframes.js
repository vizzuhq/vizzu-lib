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
              title: 'Jumping from 0% to 50% progress at the begining of the animation',
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
          chart = chart.animate({
            config: {
              channels: {
                x: {
                  attach: 'Kinds'
                },
                y: {
                  detach: 'Kinds'
                }
              }
            }
          })
          chart.activated.then((control) => control.seek('50%'))
          return chart
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: 'Using initial animation control parameters'
            }
          })
        },
        (chart) => {
          chart = chart.animate(
            {
              config: {
                channels: {
                  x: {
                    detach: ['Kinds']
                  },
                  y: {
                    attach: ['Kinds']
                  }
                }
              }
            },
            {
              playState: 'paused',
              position: 0.5
            }
          )
          chart.activated.then((control) => control.play())
          return chart
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: 'Using keyframes'
            }
          })
        },
        (chart) => {
          return chart.animate([
            {
              target: {
                config: {
                  channels: {
                    x: {
                      attach: ['Kinds']
                    },
                    y: {
                      detach: ['Kinds']
                    }
                  },
                  title: 'Using keyframes'
                }
              },
              options: {
                duration: 0.5
              }
            },
            {
              target: {
                config: {
                  channels: {
                    x: {
                      detach: ['Kinds']
                    },
                    y: {
                      attach: ['Kinds']
                    }
                  }
                }
              },
              options: {
                duration: 1
              }
            }
          ])
        }
      ]
    }
  ])
})
