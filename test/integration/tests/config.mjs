var ConfigCollection = {
    simpleData: {
        channels: {
            x: { attach: [ "Foo"] },
            y: { attach: [ "Bar"] }
        },
        title: null,
        legend: null,
    },
    simpleDataCircleGeometry: {
        geometry: 'circle',
        channels: {
            x: { attach: [ "Foo"] },
            y: { attach: [ "Bar"] }
        },
        title: null,
        legend: null,
    },
    simpleDataWithInvertAxis: {
        channels: {
            x: { attach: [ "Bar"] },
            y: { attach: [ "Foo"] }
        },
        title: null,
        legend: null,
    }
}


export default ConfigCollection;