export default class Presets {
	constructor(chart) {
		this._presetConfigs = {
			'column': {
				channels: {
					x: 'x',
					y: 'y',
					label: 'y'
				}
			},
			'groupedColumn': {
				channels: {
					x: ['x', 'group'],
					y: 'y',
					label: 'y',
					color: 'x'
				}
			},
			'stackedColumn': {
				channels: {
					x: 'x',
					y: ['y', 'stack'],
					label: 'y',
					color: 'stack'
				}
			},
			'splittedColumn': {
				channels: {
					x: 'x',
					y: ['y', 'split'],
					color: 'split',
				},
				split: true
			},
			'ratioColumn': {
				channels: {
					x: 'x',
					y: ['y', 'stack'],
					color: 'stack',
				},
				align: 'stretch'
			},
			'stackedMekko': {
				channels: {
					x: ['x', 'xStack'],
					y: ['y', 'yStack'],
					color: 'yStack',
					label: 'xStack'
				}
			}
		};

		for (let key in this._presetConfigs) {
			this[key] = (config) => {
				return this._buildPresetConfig(key, config);
			}
		}
	}

	_nullConfig() {
		return {
			align: 'none',
			coordSystem: 'cartesian',
			orientation: 'horizontal',
			rotate: 0,
			split: false,
			channels: {
				x: null,
				y: null,
				color: null,
				lightness: null,
				size: null,
				noop: null,
				label: null,
			},
			legend: 'auto',
			title: '',
			reverse: false,
			sort: 'none',
		};
	}

	_createPresetConfig(presetName) {
		let presetConfig = this._presetConfigs[presetName];
		let base = Object.assign(this._nullConfig(), presetConfig);
		return base;
	}

	_getChannelCopy(channel)
	{
		if (Array.isArray(channel)) {
			return channel.map(v => v);
		} else {
			return [ channel ];
		}
	}

	_fillChannels(presetConfig, config) {
		if (!config) return;
		let channels = presetConfig.channels;
		for (let channel in channels) {
			if (typeof channels[channel] === 'string') {
				channels[channel] = 
					this._getChannelCopy(config[channels[channel]]);
			}
			else if (Array.isArray(channels[channel])) {
				channels[channel] = channels[channel]
					.map(v => this._getChannelCopy(config[v])).flat();
			}
		}
	}

	_setupUserParams(base, config) {
		['legend', 'title', 'reverse', 'sort'].forEach(
			key => {
				if (config[key] !== undefined) {
					base[key] = config[key];
				}
			}
		);
	}

	_buildPresetConfig(presetName, config) {
		let presetConfig = this._createPresetConfig(presetName);
		this._fillChannels(presetConfig, config);
		this._setupUserParams(presetConfig, config);
		return presetConfig;
	}
}
