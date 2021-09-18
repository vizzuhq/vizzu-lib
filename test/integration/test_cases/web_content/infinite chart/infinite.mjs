import { data } from '/test/integration/test_cases/web_content/infinite chart/infinite_data.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels:
            {
                x: { attach: ['Hor'], range: { max: '695' } },
                y: { attach: ['Ver'], range: { max: '375' } },
                color: { set: ['Joy factors'] }
            },
            align: 'none',
            geometry: 'circle',
            legend: null
        },
        style: {
            plot: {
                marker: {
                    circleMinRadius: 0.0760,
                    colorPalette: '#ee1b22FF #cecf07FF #01abf6FF #0ca749FF',
                    label: {
                        fontSize: 8
                    }
                },
                xAxis: {
                    color: '#ffffff00',
                    title: { color: '#ffffff00' },
                    label: { color: '#ffffff00' },
                    interlacing: { color: '#ffffff00' },
                    ticks: { color: '#ffffff00' },
                    guides: { color: '#ffffff00' }
                },
                yAxis: {
                    color: '#ffffff00',
                    title: { color: '#ffffff00' },
                    label: { color: '#ffffff00' },
                    interlacing: { color: '#ffffff00' },
                    ticks: { color: '#ffffff00' },
                    guides: { color: '#ffffff00' }
                }
            }
        }
    }
    ),
    chart => chart.animate({
        config: {
            channels: {
                x: { detach: ['Hor'], range: { max: '119.17%' } },
                y: { detach: ['Ver'], range: { max: '127.86%' } },
                size: { set: ['Value 1'] }
            },
            title: '2'
        },
        style: {
            plot: {
                marker: {
                    circleMinRadius: null
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                size: { set: ['Value 1', 'Country code'] }
            },
            title: '3',
            legend: 'color'
        },
        style: {
            plot {
        marker: {
            circleMinRadius: null
        },
        xAxis: {
            color: null,
            title: { color: null },
            label: { color: null },
            interlacing: { color: null },
            ticks: { color: null },
            guides: { color: null }
        },
        yAxis: {
            color: null,
            title: { color: null },
            label: { color: null },
            interlacing: { color: null },
            ticks: { color: null },
            guides: { color: null }
        }
    }
            }
        }
        , '100ms'),
chart => chart.animate({
    config: {
        channels: {
            label: { set: ['Country code'] }
        },
        title: '4'
    }
}
    , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 1',], range: { max: '110%' } },
                y: { set: ['Value 3'], range: { max: '110%' } },
                size: { set: ['Value 1'] },
                noop: { set: ['Country code'] }
            },
            title: '5'
        },
        style: {
            plot: {
                marker: {
                    circleMinRadius: 0.001,
                    circleMaxRadius: 0.04,
                    label:
                    {
                        fontSize: null
                    }
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                y: { set: ['Joy factors'], range: { min: '-10%', max: '110%' } },
                label: { set: null }
            },
            title: '6',
            geometry: 'rectangle',
            orientation: 'vertical'
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: 1
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                noop: { detach: ['Country code'] },
                x: { set: ['Country code'], range: { max: '100%' } },
                y: { set: ['Value 1', 'Joy factors'], range: { min: '-10%', max: '110%' } }
            },
            title: '7'
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: null
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                y: { attach: ['Region'] },
                lightness: { set: ['Region'] }
            },
            title: '8'
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Region', 'Country code'] },
                y: { set: ['Value 1', 'Joy factors'] }
            },
            title: '9'
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 1', 'Region', 'Country code'], range: { max: '110%' } },
                y: { set: ['Joy factors'], range: { min: '-15%', max: '110%' } }
            },
            title: '10'
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 1', 'Region', 'Country code'], range: { max: '133%' } },
                y: { set: ['Joy factors'], range: { min: '-30%', max: '100%' } }
            },
            title: '11',
            coordSystem: 'polar',
            legend: 'lightness'
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 1', 'Country code'] },
                y: { set: ['Joy factors', 'Region'], range: { min: '-10%', max: '100%' } }
            },
            title: '12'
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 1'], range: { max: '110%' } },
                y: { set: ['Joy factors', 'Region', 'Country code'], range: { min: '-10%', max: '100%' } }
            },
            title: '13'
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 1', 'Joy factors', 'Region', 'Country code'], range: { max: '100%' } },
                y: { set: null, range: { min: '-200%', max: '100%' } },
                lightness: { set: null }
            },
            title: '14',
            legend: 'color'
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: null
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 1', 'Joy factors', 'Region', 'Country code'], range: { max: '100%' } },
                y: { set: null, range: { min: '0%', max: '100%' } }
            },
            title: '15'
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: null
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 1', 'Joy factors', 'Region', 'Country code'], range: { max: '100%' } },
                y: { set: ['Value 1'], range: { min: '-80%', max: '100%' } }
            },
            title: '16'
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: null
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors', 'Region'], range: { max: '100%' } },
                y: { set: ['Value 1', 'Country code'], range: { min: '-20%', max: '100%' } }
            },
            title: '18'
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: 0.1
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors'], range: { max: '100%' } },
                y: { set: ['Value 1', 'Region', 'Country code'], range: { min: '0%', max: '100%' } }
            },
            title: '19'
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: 0
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 1', 'Joy factors', 'Region', 'Country code'], range: { max: '100%' } },
                y: { set: null, range: { min: '-200%', max: '100%' } }
            },
            title: '20',
            coordSystem: 'cartesian'
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: 0
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 1', 'Joy factors', 'Country code'], range: { max: '100%' } }, // regiont levesz
                y: { set: null, range: { min: '-200%', max: '100%' } }
            },
            title: '21',
            coordSystem: 'cartesian'
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: null
                }
            }
        }
    }
        , '400ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors', 'Country code'], range: { max: '100%' } },
                y: { set: ['Value 1'], range: { min: '0%', max: '110%' } }
            },
            title: '22'
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: 1
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors', 'Country code'], range: { max: '100%' } },
                y: { set: ['Value 1', 'Age group'], range: { min: '0%', max: '110%' } },
                color: { set: ['Age group'] }
            },
            title: '23',
            legend: 'lightness'
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: 1,
                    colorPalette: null
                }
            }
        }
    }
        , '100ms'),
    chart => chart.animate({
        config: {
            channels:
            {
                x: { set: ['Joy factors', 'Country code'], range: { max: '100%' } },
                y: { set: ['Value 1', 'Age group'], range: { min: '0%', max: '100%' } }
            },
            title: '24',
            split: true,
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: 1
                }
            }
        }
    }),
    chart => chart.animate({
        config: {
            channels:
            {
                x: { set: ['Joy factors', 'Country code'], range: { max: '100%' } },
                y: { set: ['Value 1', 'Age group'], range: { min: '0%', max: '100%' } }
            },
            title: '25',
            geometry: 'area',
            align: 'stretch',
            split: false
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: 0
                }
            }
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors', 'Country code'], range: { max: '100%' } },
                y: { set: ['Value 1', 'Age group'], range: { min: '0%', max: '100%' } }
            },
            title: '26',
            geometry: 'area',
            align: 'center',
            split: true
        },
        style:
        {
            plot:
            {
                marker:
                {
                    rectangleSpacing: 0
                }
            }
        }
    }),
    chart => chart.animate({
        config: {
            channels:
            {
                x: { set: ['Joy factors', 'Country code'], range: { max: '100%' } },
                y: { set: ['Value 1', 'Age group'], range: { min: '0%', max: '100%' } }
            },
            title: '26',
            geometry: 'area',
            align: 'center',
            split: false
        },
        style: {
            plot: {
                marker: {
                    rectangleSpacing: 0
                }
            }
        }
    })






];

export default testSteps;