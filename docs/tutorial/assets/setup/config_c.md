```javascript
chart.animate({
    data: data,
    config: {
        channels: {
            y: {
                set: ['Popularity', 'Kinds']
            },
            x: {
                set: ['Genres']
            },
            color: {
                set: ['Kinds']
            },
            label: {
                set: ['Popularity']
            },
        },
    }
})
```
