# Writing plugins

## What is a plugin

A plugin is a custom class that can change the chart config or data in runtime
or it can even modify the displayed chart. It works by providing functions to
the chart instance which will be called automatically or manually at certain
points in the animation process. Plugins are recommended for custom charts when
the available configuration options are not enough.

The functions of the plugin can be **hooks** which are called automatically at
certain points during the rendering, **API functions** which can be called
manually or **listeners** that are connected to certain (predefined) events.

## How to write a plugin

To write a plugin, first import the `Plugins` module.

```typescript
import { type Plugins } from 'vizzu'
```

Create the plugin class implementing the `Plugins.Plugin` interface.

```typescript
export class ExamplePlugin implements Plugins.Plugin {
}
```

The class can have a `meta` property which provides the basic configuration for
the plugin. It has the following fields:

- `name`: the name of the plugin
- `version`: the version of the **library** the plugin is compatible with
- `depends`: an array of strings describing other plugins this one depends on

```typescript
export class ExamplePlugin implements Plugins.Plugin {
    meta = {
        name: 'examplePlugin',
        version: 'xLIB_PATCH_VERSIONx',
        depends: []
    }
}
```

The main functionality of the plugin is provided by the `hooks` property. Use a
getter function to define the object. There are three plugin hooks which can be
defined in the returned object:

- `prepareAnimation`: Called when the animate() parameters gets set in the
    library to prepare the animation.
- `registerAnimation`: Called when the animate() method called, and the lib
    schedules the call to the animation queue.
- `runAnimation`: Called when all animate() parameter set and animation can be
    started.

Let’s add a simple function to the `prepareAnimation` phase. We’d like to
automatically set the chart title to the name of the data series which is
assigned to the color channel (if any). If no color is selected, then set a
default title.

### Adding hooks

```typescript
export class ExamplePlugin implements Plugins.Plugin {
    get hooks(): Plugins.PluginHooks {
        return {
            prepareAnimation: (ctx: Plugins.PrepareAnimationContext, next: () => void): void => {
                const config = ctx.target[0].target.config
                if (config.channels.color) {
                    config.title = Object.values(config.channels.color)[0][0]
                    this.hasChanged = true
                } else {
                    config.title = 'Example plugin'
                }
                next()
            }
        }
    }
}
```

The hooks can also have a `priority` number that decides the order the
registered plugins run. It’s a float value, ranging from 0 (last) to 1 (first).

```typescript
export class ExamplePlugin implements Plugins.Plugin {
    get hooks(): Plugins.PluginHooks {
        return {
            prepareAnimation: Object.assign(
                (ctx: Plugins.PrepareAnimationContext, next: () => void): void => {
                    const config = ctx.target[0].target.config
                    if (config.channels.color) {
                        config.title = Object.values(config.channels.color)[0][0]
                        this.hasChanged = true
                    } else {
                        config.title = 'Example plugin'
                    }
                    next()
                },
                { priority: .9 }
            )
        }
    }
}
```

### Adding API functions

The class can also define an `api` property. It can contain one or more
functions which can be called from the original context using
`chart.feature.<plugin name>.<function name>()`.

Let’s add an API function to our plugin which can tell if the default title has
been changed.

```typescript
import { type Plugins } from 'vizzu'

interface ExamplePluginApi extends Plugins.PluginApi {
    hasDefaultTitle(): boolean
}

export class ExamplePlugin implements Plugins.Plugin {
    meta: Plugins.PluginMeta = {
        name: 'examplePlugin',
        version: 'xLIB_PATCH_VERSIONx',
        depends: []
    }
    private hasChanged: boolean = false

    get hooks(): Plugins.PluginHooks {
        return {
            prepareAnimation: Object.assign(
                (ctx: Plugins.PrepareAnimationContext, next: () => void): void => {
                    const config = ctx.target[0].target.config
                    if (config.channels.color) {
                        config.title = Object.values(config.channels.color)[0][0]
                        this.hasChanged = true
                    } else {
                        config.title = 'Example plugin'
                    }
                    next()
                },
                { priority: .9 }
            )
        }
    }

    get api(): ExamplePluginApi {
        return {
            hasDefaultTitle: (): boolean => {
                return !this.hasChanged
            }
        }
    }
}
```

### Listening to events

Define a getter method for the listeners property as follows:

```typescript
get listeners(): Plugins.PluginListeners {
    return {
        [Events.EventType.pointerup]: (): void => {
            console.log('You clicked on the chart')
        }
    }
}
```

The proper functions will be called when the defined event happens in the chart.

## How to use plugins

After creating a new `Vizzu` instance, enable the plugin using the `feature()`
method like this:

```javascript
const chart = new Vizzu('vizzu')
chart.feature(new ExamplePlugin(), true)
```

The first parameter is a plugin instance, the second one is the enabled status
(`true` for enabled and `false` for disabled.) If the plugin is enabled, then
the hook functions will be automatically called at the proper animation phases.

The API functions must be called manually using the chart instance. Example:

```javascript
anim.activated.then(() => {
    if (chart.feature.examplePlugin.hasDefaultTitle()) {
        console.log(`the title has not been changed`)
    } else {
        console.log(`the title has been changed`)
    }
})
```

!!! info

    For more information check out our plugins at
    <https://github.com/vizzuhq/vizzu-lib-ext>.
