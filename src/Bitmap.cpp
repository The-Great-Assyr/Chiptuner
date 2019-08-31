#include "Bitmap.hpp"

void MFTexture::reload(NVGcontext *vg, std::string fileName, int imageFlags) {
	if (image)
		nvgDeleteImage(vg, image);
	image = nvgCreateImage(vg, fileName.c_str(), imageFlags);
	name = fileName;
	context = vg;
	refCount++;
	if (!image)
		return;
	nvgImageSize(vg, image, &width, &height);
}
void MFTexture::release() {
	refCount--;
	if (refCount)
		return;
	refCount = 0;
	if (image)
		nvgDeleteImage(context, image);
	image = 0;
	debug("Image Released %s", name.c_str());
}

std::shared_ptr<MFTexture> MFTextureList::load(NVGcontext *vg, std::string fileName, int imageFlags) {
	for (std::shared_ptr<MFTexture> tex : list) {
		if ((tex->context == vg) && !tex->name.compare(fileName)) {
			if (tex->image) {
				tex->refCount++;
				return tex;
			}
			tex->reload(vg, fileName, imageFlags);	
			return tex;
		}
	}
	std::shared_ptr<MFTexture> tex = std::make_shared<MFTexture>(vg, fileName, imageFlags);
	list.push_back(tex);
	debug("Image loaded %s", fileName.c_str());
	return tex;
}

MFTextureList gTextureList;

void BitMap::DrawImage(const DrawArgs &args) {
	if (!loaded) {
		loaded = true;
		bitmap = gTextureList.load(args.vg, path, 0);
		if (!bitmap->image)
			warn("ChipTuner: Unable to load %s", path.c_str());
	}
	if (!bitmap->image)
		return;	
	NVGpaint paint = nvgImagePattern(args.vg, 0, 0, box.size.x, box.size.y, 0.0f, bitmap->image, 1.0f);
	nvgFillPaint(args.vg, paint);
	nvgBeginPath(args.vg);
	nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
	nvgFill(args.vg);
	
}
void BitMap::draw(const DrawArgs &args) {
	DrawImage(args);
	TransparentWidget::draw(args);
}
