#include "ChipTuner.hpp"

struct BlankBaseWidget : ModuleWidget {
	static constexpr int LISTSIZE = 3;
	int selected = 0;
	std::string fileName[LISTSIZE];
	BitMap *bmp;
	std::string FileName(std::string tpl, int templateSize) {
		char workingSpace[100];
		snprintf(workingSpace, 100, tpl.c_str(), templateSize);
		return asset::plugin(pluginInstance, workingSpace);
	}

	BlankBaseWidget(Module *module) : ModuleWidget() {
		setModule(module);
	}
	void appendContextMenu(Menu *menu) override;
	void loadBitmap() {
		bmp = createWidget<BitMap>(Vec(0,0));
		bmp->box.size.x = box.size.x;
		bmp->box.size.y = box.size.y;
		bmp->path = fileName[selected];
		addChild(bmp);
	}
	void setBitmap(int sel) {
		if (selected == sel)
			return;
		selected = clamp(sel, 0, LISTSIZE - 1);
		removeChild(bmp);
		delete bmp;
		loadBitmap();
	}
	
};

struct BitmapMenuItem : MenuItem {
	BlankBaseWidget *w;
	int value;
	void onAction(const event::Action &e) override {
		w->setBitmap(value);
	}
};

void BlankBaseWidget::appendContextMenu(Menu *menu) {
	menu->addChild(new MenuEntry);
	BitmapMenuItem *m = createMenuItem<BitmapMenuItem>("Base");
	m->w = this;
	m->value = 0;
	m->rightText = CHECKMARK(selected==m->value);
	menu->addChild(m);
	m = createMenuItem<BitmapMenuItem>("Alternative");
	m->w = this;
	m->value = 1;
	m->rightText = CHECKMARK(selected==m->value);
	menu->addChild(m);
	m = createMenuItem<BitmapMenuItem>("Extreme");
	m->w = this;
	m->value = 2;
	m->rightText = CHECKMARK(selected==m->value);
	menu->addChild(m);	
}

template<int x>
struct BlankWidget : BlankBaseWidget {
	BlankWidget(Module *module) : BlankBaseWidget(module) {
		fileName[0] = FileName("res/8Bit_%dHP.png", x);
		fileName[1] = FileName("res/8Bit_Alt_%dHP.png", x);
		fileName[2] = FileName("res/8Bit_Ext_%dHP.png", x);		
		box.size = Vec(RACK_GRID_WIDTH * x, RACK_GRID_HEIGHT);
		loadBitmap();
	}
};

Model *modelBlank_1HP = createModel<Module, BlankWidget<1>>("Blank1HP");
Model *modelBlank_2HP = createModel<Module, BlankWidget<2>>("Blank2HP");
Model *modelBlank_3HP = createModel<Module, BlankWidget<3>>("Blank3HP");
Model *modelBlank_4HP = createModel<Module, BlankWidget<4>>("Blank4HP");
Model *modelBlank_6HP = createModel<Module, BlankWidget<6>>("Blank6HP");
Model *modelBlank_10HP = createModel<Module, BlankWidget<10>>("Blank10HP");
Model *modelBlank_12HP = createModel<Module, BlankWidget<12>>("Blank12HP");
Model *modelBlank_16HP = createModel<Module, BlankWidget<16>>("Blank16HP");
Model *modelBlank_20HP = createModel<Module, BlankWidget<20>>("Blank20HP");
Model *modelBlank_26HP = createModel<Module, BlankWidget<26>>("Blank26HP");
Model *modelBlank_32HP = createModel<Module, BlankWidget<32>>("Blank32HP");
