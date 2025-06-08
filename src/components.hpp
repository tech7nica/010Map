#pragma once
#include <string>
#include "plugin.hpp"

struct LJack : app::SvgPort {
	LJack() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/LJack.svg")));
	}
};

struct RJack : app::SvgPort {
	RJack() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/RJack.svg")));
	}
};

struct CVJack : app::SvgPort {
	CVJack() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/CVJack.svg")));
	}
};

struct GateJack : app::SvgPort {
	GateJack() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/GateJack.svg")));
	}
};

struct ModJack : app::SvgPort {
	ModJack() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/ModJack.svg")));
	}
};

struct BlackScrew : app::SvgScrew {
	BlackScrew() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/BlackScrew.svg")));
	}
};

struct LargeKnob : RoundKnob {
	LargeKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/LargeKnob.svg")));
		bg->setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/LargeKnob_bg.svg")));
	}
};

struct SmallKnob : RoundKnob {
	SmallKnob() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/SmallKnob.svg")));
		bg->setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/SmallKnob_bg.svg")));
	}
};

