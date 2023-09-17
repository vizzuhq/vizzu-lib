import { data } from '../../../test_data/chart_types_eu.mjs'
data.filter = (record) =>
  record.Country != 'Germany' &&
  record.Country != 'Italy' &&
  record.Country != 'Hungary' &&
  record.Country != 'Netherlands' &&
  record.Country != 'Estonia' &&
  record.Country != 'Belgium' &&
  record.Country != 'Sweden' &&
  record.Country != 'Poland' &&
  record.Country != 'Malta' &&
  record.Country != 'Romania' &&
  record.Country != 'Greece' &&
  record.Country != 'Slovakia' &&
  record.Country != 'Ireland' &&
  record.Country != 'Lithuania' &&
  record.Country != 'Croatia' &&
  record.Country != 'Slovenia' &&
  record.Country != 'Portugal' &&
  record.Country != 'Finland' &&
  record.Country != 'United Kingdom'

const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: {
        channels: {
          y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
          x: { attach: ['Country'] },
          color: { attach: ['Country'] },
          label: { attach: ['Value 5 (+/-)'] }
        },
        title: 'Rectangle Marker Label Positions - Center'
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Top',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'top',
              orientation: 'horizontal'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Left',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'left'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Bottom',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'bottom'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Right',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'right'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Center/Horizontal',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'center'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Center/Tangential',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              orientation: 'tangential'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Center/Normal',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              orientation: 'normal'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Center/Vertical',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              orientation: 'vertical'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        channels: {
          y: { attach: ['Value 2 (+)'] },
          label: { detach: ['Value 5 (+/-)'] },
          label: { attach: ['Value 2 (+)'] }
        },
        title: 'Change Continuous',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'top',
              orientation: 'horizontal'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        channels: {
          y: { detach: ['Value 2 (+)'], range: { min: null, max: null } },
          x: { attach: ['Value 2 (+)'] },
          label: { detach: ['Value 5 (+/-)'] },
          label: { attach: ['Value 2 (+)', 'Country'] }
        },
        title: 'Polar Coordinate',
        rotate: 180,
        coordSystem: 'polar'
      },
      style: {
        plot: {
          marker: {
            label: {
              orientation: 'horizontal'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Left',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'left'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Bottom',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'bottom'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Right',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'right'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Center/Horizontal',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'center',
              orientation: 'horizontal'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Top/Normal',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'top',
              orientation: 'normal'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Top/Vertical',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              orientation: 'vertical'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Top/Tangential',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              orientation: 'tangential'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Top/Angle',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              angle: 0.5 * 3.14
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { detach: ['Value 2 (+)', 'Country'] },
          size: { attach: ['Value 2 (+)'] }
        },
        title: 'Without Coordinate',
        rotate: 10,
        coordSystem: 'cartesian'
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'center',
              orientation: 'horizontal'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Center/Horizontal',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              orientation: 'horizontal',
              angle: null
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      style: {
        plot: {
          marker: {
            label: {
              position: 'top',
              orientation: 'horizontal'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Left',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'left'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Bottom',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'bottom'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Right',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'right'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Center/Horizontal',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'center',
              orientation: 'horizontal'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Top/Normal',
        rotate: 10
      },
      style: {
        plot: {
          marker: {
            label: {
              position: 'top',
              orientation: 'normal'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Top/Vertical',
        rotate: -360
      },
      style: {
        plot: {
          marker: {
            label: {
              orientation: 'vertical'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Top/Tangential',
        rotate: 100
      },
      style: {
        plot: {
          marker: {
            label: {
              orientation: 'tangential'
            }
          }
        }
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        title: 'Rectangle Marker Label Positions - Top/Angle',
        rotate: 380
      },
      style: {
        plot: {
          marker: {
            label: {
              angle: 1 * 3.14
            }
          }
        }
      }
    })
]

export default testSteps
