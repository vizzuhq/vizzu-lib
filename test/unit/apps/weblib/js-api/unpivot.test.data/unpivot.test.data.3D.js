const Data3D = {
    'data_3d_1': {
        input: {
            dimensions:
                [
                    {
                        name: 'xt',
                        values: ['A', 'B', 'C']
                    },
                    {
                        name: 'yt',
                        values: ['a', 'b']
                    },
                    {
                        name: 'zt',
                        values: ['AA', 'BB', 'CC', 'DD']
                    }
                ],
            measures:
                [
                    {
                        name: 'Foo',
                        values: [
                            // AA
                            [
                                // A  B  C
                                [1, 2, 3], // a
                                [4, 5, 6]  // b
                            ],
                            // BB
                            [
                                // A  B  C
                                [11, 12, 13], // a
                                [14, 15, 16]  // b
                            ],
                            // CC
                            [
                                // A  B  C
                                [21, 22, 23], // a
                                [24, 25, 26]  // b
                            ],
                            // DD
                            [
                                // A  B  C
                                [31, 32, 33], // a
                                [34, 35, 36]  // b
                            ]
                        ]
                    },
                    {
                        name: 'Bar',
                        type: 'measure',
                        values: [
                            // AA
                            [
                                // A  B  C
                                [41, 42, 43], // a
                                [44, 45, 46]  // b
                            ],
                            // BB
                            [
                                // A  B  C
                                [51, 52, 53], // a
                                [54, 55, 56]  // b
                            ],
                            // CC
                            [
                                // A  B  C
                                [61, 62, 63], // a
                                [64, 65, 66]  // b
                            ],
                            // DD
                            [
                                // A  B  C
                                [71, 72, 73], // a
                                [74, 75, 76]  // b
                            ]
                        ]
                    }
                ]
        },
        output: {
            series:
                [
                    {
                        name: 'xt',
                        type: 'dimension',
                        values: ['A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C']
                    },
                    {
                        name: 'yt',
                        type: 'dimension',
                        values: ['a', 'a', 'a', 'b', 'b', 'b', 'a', 'a', 'a', 'b', 'b', 'b', 'a', 'a', 'a', 'b', 'b', 'b', 'a', 'a', 'a', 'b', 'b', 'b']
                    },
                    {
                        name: 'zt',
                        type: 'dimension',
                        values: ['AA', 'AA', 'AA', 'AA', 'AA', 'AA', 'BB', 'BB', 'BB', 'BB', 'BB', 'BB', 'CC', 'CC', 'CC', 'CC', 'CC', 'CC', 'DD', 'DD', 'DD', 'DD', 'DD', 'DD']
                    },
                    {
                        name: 'Foo',
                        values: [1, 2, 3, 4, 5, 6, 11, 12, 13, 14, 15, 16, 21, 22, 23, 24, 25, 26, 31, 32, 33, 34, 35, 36]
                    },
                    {
                        name: 'Bar',
                        type: 'measure',
                        values: [41, 42, 43, 44, 45, 46, 51, 52, 53, 54, 55, 56, 61, 62, 63, 64, 65, 66, 71, 72, 73, 74, 75, 76]
                    }
                ]
        }
    }
};

export default Data3D