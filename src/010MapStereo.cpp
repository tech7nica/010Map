// 010MapStereo.cpp
// Copyright © 2025 tech7nica All Rights Reserved.

#include "plugin.hpp"
#include "components.hpp"


struct _010MapStereo : Module {
	enum ParamId {
		START1KNOB_PARAM,
		STOP1KNOB_PARAM,
		START2KNOB_PARAM,
		STOP2KNOB_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		START1MODIN_INPUT,
		STOP1MODIN_INPUT,
		START2MODIN_INPUT,
		STOP2MODIN_INPUT,
		LEFTIN_INPUT,
		RIGHTIN_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		LEFTOUT_OUTPUT,
		RIGHTOUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	_010MapStereo() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(START1KNOB_PARAM, -10.f, 10.f, -10.f, "");
		configParam(STOP1KNOB_PARAM, -10.f, 10.f, 10.f, "");
		configParam(START2KNOB_PARAM, -10.f, 10.f, 0.f, "");
		configParam(STOP2KNOB_PARAM, -10.f, 10.f, 0.f, "");
		configInput(START1MODIN_INPUT, "start1");
		configInput(STOP1MODIN_INPUT, "stop1");
		configInput(START2MODIN_INPUT, "start2");
		configInput(STOP2MODIN_INPUT, "stop2");
		configInput(LEFTIN_INPUT, "Left Input");
		configInput(RIGHTIN_INPUT, "Right Input");
		configOutput(LEFTOUT_OUTPUT, "Left Output");
		configOutput(RIGHTOUT_OUTPUT, "Right Output");
	}

	/** 渡された数値をある範囲から別の範囲に変換
	引用：https://qiita.com/mao_/items/0bcae49d4ea1a5ac0cf0
	value:変換する入力値
	start1:現在の範囲の下限
	stop1:現在の範囲の上限
	start2:変換する範囲の下限
	stop2:変換する範囲の上限
	return:変換後の値
	*/
	float map(float value, float start1, float stop1, float start2, float stop2)
	{
		// 現在の範囲が0の場合は変換できないので0を返して終了
		if (stop1 == start1) return 0.f;
		// map関数の計算式
		float out = start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
		// 無限大やNaNが出た場合は0を返す
		if (!std::isfinite(out)) out = 0.f;
		return out;
	}

	void process(const ProcessArgs& args) override {

		// ポリフォニック対応
		int channels = std::max(inputs[LEFTIN_INPUT].getChannels(), 1);

		// チャンネル数分ループ
		for (int c = 0; c < channels; c++) {
				
			// 入力信号を読む
			float signalLeftIn = inputs[LEFTIN_INPUT].getPolyVoltage(c);
			float signalRightIn = inputs[RIGHTIN_INPUT].getPolyVoltage(c);

			// Start1ノブの値を読む（Mod入力がある場合そちらを優先する）
			float start1Vol = params[START1KNOB_PARAM].getValue();
			if (inputs[START1MODIN_INPUT].isConnected()){
				start1Vol = inputs[START1MODIN_INPUT].getPolyVoltage(c);
			}

			// Stop1ノブの値を読む（Mod入力がある場合そちらを優先する）
			float stop1Vol = params[STOP1KNOB_PARAM].getValue();
			if (inputs[STOP1MODIN_INPUT].isConnected()){
				stop1Vol = inputs[STOP1MODIN_INPUT].getPolyVoltage(c);
			}

			// Start2ノブの値を読む（Mod入力がある場合そちらを優先する）
			float start2Vol = params[START2KNOB_PARAM].getValue();
			if (inputs[START2MODIN_INPUT].isConnected()){
				start2Vol = inputs[START2MODIN_INPUT].getPolyVoltage(c);
			}

			// Stop2ノブの値を読む（Mod入力がある場合そちらを優先する）
			float stop2Vol = params[STOP2KNOB_PARAM].getValue();
			if (inputs[STOP2MODIN_INPUT].isConnected()){
				stop2Vol = inputs[STOP2MODIN_INPUT].getPolyVoltage(c);
			}

			// Map関数の結果を出力変数に格納
			float signalLeftOut = map(signalLeftIn, start1Vol, stop1Vol, start2Vol, stop2Vol);
			float signalRightOut = map(signalRightIn, start1Vol, stop1Vol, start2Vol, stop2Vol);

			// 出力変数の内容を出力する
			outputs[LEFTOUT_OUTPUT].setVoltage(signalLeftOut, c);
			outputs[RIGHTOUT_OUTPUT].setVoltage(signalRightOut, c);
		}
		outputs[LEFTOUT_OUTPUT].setChannels(channels);
		outputs[RIGHTOUT_OUTPUT].setChannels(channels);
	}
};


struct _010MapStereoWidget : ModuleWidget {
	_010MapStereoWidget(_010MapStereo* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/010MapStereo.svg")));

		addChild(createWidget<BlackScrew>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<BlackScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<BlackScrew>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<BlackScrew>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<SmallKnob>(mm2px(Vec(8.513, 33.81)), module, _010MapStereo::START1KNOB_PARAM));
		addParam(createParamCentered<SmallKnob>(mm2px(Vec(21.967, 33.756)), module, _010MapStereo::STOP1KNOB_PARAM));
		addParam(createParamCentered<SmallKnob>(mm2px(Vec(8.513, 66.75)), module, _010MapStereo::START2KNOB_PARAM));
		addParam(createParamCentered<SmallKnob>(mm2px(Vec(21.967, 66.696)), module, _010MapStereo::STOP2KNOB_PARAM));

		addInput(createInputCentered<ModJack>(mm2px(Vec(8.513, 44.003)), module, _010MapStereo::START1MODIN_INPUT));
		addInput(createInputCentered<ModJack>(mm2px(Vec(21.967, 43.948)), module, _010MapStereo::STOP1MODIN_INPUT));
		addInput(createInputCentered<ModJack>(mm2px(Vec(8.513, 76.943)), module, _010MapStereo::START2MODIN_INPUT));
		addInput(createInputCentered<ModJack>(mm2px(Vec(21.967, 76.889)), module, _010MapStereo::STOP2MODIN_INPUT));
		addInput(createInputCentered<LJack>(mm2px(Vec(8.513, 95.175)), module, _010MapStereo::LEFTIN_INPUT));
		addInput(createInputCentered<RJack>(mm2px(Vec(21.967, 95.175)), module, _010MapStereo::RIGHTIN_INPUT));

		addOutput(createOutputCentered<LJack>(mm2px(Vec(8.513, 113.364)), module, _010MapStereo::LEFTOUT_OUTPUT));
		addOutput(createOutputCentered<RJack>(mm2px(Vec(21.967, 113.364)), module, _010MapStereo::RIGHTOUT_OUTPUT));
	}
};


Model* model_010MapStereo = createModel<_010MapStereo, _010MapStereoWidget>("010MapStereo");
