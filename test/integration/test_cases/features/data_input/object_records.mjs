const data = {
    series: [
        {
           name: "Dim0",
           type: "dimension",
        },
        {
            name: "Dim1",
            type: "dimension",
        },
        {
            name: "Meas",
            type: "measure",
        }
    ],
    records: [
        { Dim0: "a", Dim1: "A" },
        { Dim0: "b", Dim1: "B", Meas: 2 },
        { Dim1: "C", Dim0: "c", Meas: 3 },
        { Dim0: "d", Meas: 4, Dim1: "D" },
    ]
}

const testSteps = [
  chart => chart.animate({ data }),
  chart => chart.animate({ x: ["Dim0", ,"Dim1"], y: "Meas", label: ["Meas", ,"Dim1"] }),
  chart => chart.animate({ data: { records: [ { Dim0: "a", Dim1: "A", Meas: 1 } ]}}),
  chart => chart.animate({ data: { records: [ { Dim1: "E", Meas: 5, Dim0: "e" } ]}}),
];

export default testSteps;
