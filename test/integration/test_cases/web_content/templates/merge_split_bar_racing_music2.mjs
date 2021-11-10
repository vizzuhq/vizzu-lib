import Vizzu from
    'https://cdn.jsdelivr.net/npm/vizzu@0.3.1/dist/vizzu.min.js';

import { data } from '/test/integration/test_data/music_industry_history (1).js';

let chart = new Vizzu('vizzuCanvas', { data });

for (let year = 1973; year <= 2020; year++) {

    chart.animate({
        data:
        {
            filter: record => parseInt(record.Year) == year
        },
        config: {
            channels: {
                y: { set: ['Format'], },
                x: { set: ['Revenue [m$]'] },
                color: { attach: ['Format'] }
            },
            title: year + ': Music Revenue by Format in the USA',
            sort: 'byValue'
        },
        style:
        {
//            fontSize: 25,
            title:
            {
                fontWeight: 200
            },
            plot: {
 //               paddingLeft: 100,
 //               paddingTop: 50,
                yAxis: {
                    color: '#ffffff00',
                    label: {
                        paddingRight: 20
                    },
                },
                xAxis: {
                    title: { color: '#ffffff00' },
                    label: { numberFormat: 'grouped' }
                },
                marker: {
                    colorPalette: '#b74c20FF #c47f58FF #1c9761FF #ea4549FF #875792FF #3562b6FF #ee7c34FF #efae3aFF'
                }
            }
        }
    },
        {
            duration: 0.1, delay: 0,
            x: { easing: 'linear', delay: 0 },
            y: { delay: 0 },
            show: { delay: 0 },
            hide: { delay: 0 },
            title: { duration: 0, delay: 0 }
        }
    )
}

chart.animate({

    config: {
        channels: {
            x: { attach: ['Year'] }
        }
    }
},
    {
        delay: 2.5,
        duration: 0
    }
)

chart.animate({
    data:
    {
        filter: record => record.Year == '2020' || record.Year == '1972'
    },
    config: {
        title: 'Lets see the total of the last 47 years'
    }
},
    {
        delay: 0,
        duration: 2
    }
)

chart.animate({

    config: {
        sort: 'none'
    }
},
    {
        delay: 0,
        duration: 2
    }
)

for (let year = 2020; year >= 1973; year--) {

    chart.animate({
        data:
        {
            filter: record => parseInt(record.Year) >= year || record.Year == '1972'
        },
        config: {
            split: true
        }
    },
        {
            delay: 0,
            duration: 0.005
        }
    )
}

chart.animate({
    data:
    {
        filter: record => record.Year != '1972'
    },
    config: {
        split: false
    }
},
    {
        delay: 0,
        duration: 1.5
    }
)

chart.animate({
    config: {
        channels: {
            x: { detach: ['Year'] }
        }
    }
},
    {
        delay: 0,
        duration: 0
    }
)

chart.animate({
    config: {
        channels: {
            label: { set: ['Revenue [m$]'] }
        }
    }
},
    {
        delay: 0,
        duration: 0.1
    }
)

chart.animate({
    config: {
        channels: {
            x: { attach: ['Year'] },
            label: { detach: ['Revenue [m$]'] }
        }
    }
},
    {
        delay: 4,
        duration: 1
    }
)

chart.animate({
    config: {
        channels: {
            x: { set: ['Year'] },
            y: { set: ['Revenue [m$]', 'Format'], range: { min: null, max: null } },
            color: { set: ['Format'] }
        },
        title: '1973-2020 - Music Revenue by Format in the USA',
        split: true
    },
    style: {
        plot: {
            paddingLeft: 13,
            paddingTop: 50,
            xAxis: {
                label: {
                    angle: 2.0,
                    fontSize: 14,
                    color: '#8e8e8e'
                }
            },
            yAxis: {
                interlacing: { color: '#ffffff00' },
                title: { color: '#ffffff00' },
                label: { color: '#ffffff00' }
            }
        }
    }
},
    {
        delay: 0,
        duration: 2
    })

chart.animate({
    config: {
        geometry: 'area'
    }
},
    {
        delay: 0,
        duration: 1
    })

chart.animate({
    config: {
        channels: {
            x: { set: ['Year'] },
            y: {
                range: {
                    max: '110%'
                }
            }
        },
        align: 'center',
        split: false
    },
    style: {
        plot: {
            marker: { borderWidth: 1 }
        }
    }
},
    {
        delay: 4,
        duration: 1
    })
