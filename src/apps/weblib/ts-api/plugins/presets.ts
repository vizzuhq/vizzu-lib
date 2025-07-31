import * as Data from '../types/data.js'
import * as Config from '../types/config.js'
import { recursiveCopy } from '../utils.js'
import { presetConfigs } from './presetconfigs.js'

type RawPresetConfigs = typeof presetConfigs

type PresetNames = keyof RawPresetConfigs

type PresetConfigs = {
	[Property in keyof RawPresetConfigs]: Config.Chart
}

type PresetInterface = {
	[Property in keyof RawPresetConfigs]: (config: Config.Chart) => Config.Chart
}

export default class Presets {
	private _presetConfigs: PresetConfigs

	constructor() {
		this._presetConfigs = presetConfigs as PresetConfigs

		for (const key in this._presetConfigs) {
			const name = key as PresetNames
			;(this as unknown as PresetInterface)[name] = (config: Config.Chart): Config.Chart => {
				return this._buildPresetConfig(name, config)
			}
		}
	}

	private _nullConfig(): Config.Chart {
		return {
			/*
			title: null,
			subtitle: null,
			caption: null,
			legend: 'auto',
			 */
			coordSystem: 'cartesian',
			orientation: 'auto',
			angle: 0,
			geometry: 'rectangle',
			channels: {
				x: {
					set: null,
					title: 'auto',
					// range: { min: 'auto', max: 'auto' },
					labelLevel: 'auto',
					axis: 'auto',
					labels: 'auto',
					ticks: 'auto',
					interlacing: 'auto',
					guides: 'auto',
					markerGuides: 'auto',
					step: 'auto',
					// sort: 'none',
					// reverse: false,
					align: 'none',
					split: false
				},
				y: {
					set: null,
					title: 'auto',
					// range: { min: 'auto', max: 'auto' },
					labelLevel: 'auto',
					axis: 'auto',
					labels: 'auto',
					ticks: 'auto',
					interlacing: 'auto',
					guides: 'auto',
					markerGuides: 'auto',
					step: 'auto',
					// sort: 'none',
					// reverse: false,
					align: 'none',
					split: false
				},
				color: {
					set: null,
					title: 'auto',
					// range: { min: 'auto', max: 'auto' },
					labelLevel: 'auto'
				},
				lightness: {
					set: null,
					title: 'auto',
					// range: { min: 'auto', max: 'auto' },
					labelLevel: 'auto'
				},
				size: {
					set: null,
					title: 'auto',
					// range: { min: 'auto', max: 'auto' },
					labelLevel: 'auto'
				},
				noop: {
					set: null,
					title: 'auto',
					// range: { min: 'auto', max: 'auto' },
					labelLevel: 'auto'
				},
				label: {
					set: null,
					title: 'auto',
					// range: { min: 'auto', max: 'auto' },
					labelLevel: 'auto'
				}
			}
		}
	}

	private _createPresetConfig(presetName: PresetNames): Config.Chart {
		const presetConfig = recursiveCopy(this._presetConfigs[presetName])
		const channelBase = { ...this._nullConfig().channels, ...presetConfig!.channels }
		const base = Object.assign(this._nullConfig(), presetConfig)
		base.channels = channelBase
		return base
	}

	private _getChannelCopy(channel: Data.SeriesList | null | undefined): Data.SeriesList | null {
		if (channel === null) return null
		if (channel === undefined) return null
		if (Array.isArray(channel)) return channel
		return [channel]
	}

	private _fillChannels(presetConfig: Config.Chart, config: Config.Chart | undefined): void {
		if (!config) return
		const channels = presetConfig.channels
		for (const key in channels) {
			const channelName = key as keyof Config.Channels
			const channel = channels[channelName]
			if (!this._isChannel(channel)) {
				continue
			} else if (typeof channel.set === 'string') {
				const key = channel.set as keyof Config.Chart
				channel.set = this._getChannelCopy(config[key] as Data.SeriesList)
			} else if (Array.isArray(channel.set)) {
				const newChannel = []
				for (let i = 0; i < channel.set.length; i++) {
					const key = channel.set[i]!.name as keyof Config.Chart
					const channelConfig = this._getChannelCopy(config[key] as Data.SeriesList)
					if (channelConfig !== null) {
						newChannel.push(channelConfig)
					}
				}
				channel.set = newChannel.length > 0 ? newChannel.flat() : null
			}
		}
	}

	private _isChannel(
		channel: Config.Channel | null | undefined | Data.SeriesList
	): channel is Config.Channel {
		return typeof channel === 'object' && channel !== null && 'set' in channel
	}

	private _assignProperty<T, K extends keyof T>(base: T, config: T, prop: K): void {
		const value = config[prop]
		if (value !== undefined) {
			base[prop] = value
		}
	}

	private _setupUserParams(
		base: Config.Chart,
		config: Config.Chart,
		mainAxis: keyof Config.Channels
	): void {
		;['legend', 'title', 'subtitle', 'caption'].forEach((key) => {
			const prop = key as keyof Config.Chart
			this._assignProperty(base, config, prop)
		})
		;['reverse', 'sort'].forEach((key) => {
			this._assignProperty(
				base.channels?.[mainAxis] as Config.Channel,
				config as Config.Channel,
				key as keyof Config.Channel
			)
		})
	}

	private _getMainAxis(presetName: PresetNames): keyof Config.Channels {
		const presetsWithYMainAxis = new Set([
			'bar',
			'groupedBar',
			'stackedBar',
			'splittedBar',
			'percentageBar',
			'verticalStream',
			'verticalViolin',
			'verticalLine',
			'pie',
			'radialBar',
			'radialStackedBar',
			'donut',
			'nestedDonut'
		])

		return presetsWithYMainAxis.has(presetName) ? 'y' : 'x'
	}

	private _buildPresetConfig(presetName: PresetNames, config: Config.Chart): Config.Chart {
		const presetConfig = this._createPresetConfig(presetName)
		const mainAxis = this._getMainAxis(presetName)
		this._fillChannels(presetConfig, config)
		this._setupUserParams(presetConfig, config, mainAxis)
		return presetConfig
	}
}
