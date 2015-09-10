/*
 * File:   Config.cpp
 * Author: TheArni
 *
 * Created on 16 de febrero de 2015, 22:32
 */

#include "Config.h"

#include <rapidjson/prettywriter.h>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>
#include "TaskEngine.h"
#include <string>

GraphicsConfig Config::graphics;
PhysicsConfig Config::physics;

void setInt(int& i, rapidjson::Value& val, const char* name) {
    if (val.HasMember(name) && val[name].IsInt()) {
        i = val[name].GetInt();
    }
}

void setDouble(double& d, rapidjson::Value& val, const char* name) {
    if (val.HasMember(name)) {
        if (val[name].IsDouble()) {
            d = val[name].GetDouble();
        } else if (val[name].IsInt()) {
            d = (double) val[name].GetInt();
        }
    }
}

void setBool(bool& b, rapidjson::Value& val, const char* name) {
    if (val.HasMember(name) && val[name].IsBool()) {
        b = val[name].GetBool();
    }
}

void setString(std::string& s, rapidjson::Value& val, const char* name) {
    if (val.HasMember(name) && val[name].IsString()) {
        s = val[name].GetString();
    }
}

void serializeGraphics(rapidjson::Document* doc) {
    rapidjson::Value g(rapidjson::kObjectType);
    {
        g.AddMember("width", Config::graphics.width, doc->GetAllocator());
        g.AddMember("height", Config::graphics.height, doc->GetAllocator());
        g.AddMember("fullscreen", Config::graphics.fullscreen, doc->GetAllocator());
        g.AddMember("fov", Config::graphics.fov, doc->GetAllocator());
        g.AddMember("vsync", Config::graphics.vsync, doc->GetAllocator());
        g.AddMember("borderless", Config::graphics.borderless, doc->GetAllocator());
        g.AddMember("nearPlaneClipping", Config::graphics.nearPlaneClipping, doc->GetAllocator());
        g.AddMember("renderDistance", Config::graphics.renderDistance, doc->GetAllocator());
        g.AddMember("antialiasing", Config::graphics.antialiasing, doc->GetAllocator());
        g.AddMember("bobbing", Config::graphics.bobbing, doc->GetAllocator());
        g.AddMember("modelDetail", Config::graphics.modelDetail, doc->GetAllocator());
        g.AddMember("textureDetail", Config::graphics.textureDetail, doc->GetAllocator());
        g.AddMember("fog", Config::graphics.fog, doc->GetAllocator());
        g.AddMember("anisotropicFiltering", Config::graphics.anisotropicFiltering, doc->GetAllocator());
        g.AddMember("shadowmapResolution", Config::graphics.shadowmapResolution, doc->GetAllocator());
        g.AddMember("depthBufferPrecission", Config::graphics.depthBufferPrecision, doc->GetAllocator());
    }
    doc->AddMember("graphics", g, doc->GetAllocator());
}

void serializePhysics(rapidjson::Document* doc) {
    rapidjson::Value p(rapidjson::kObjectType);
    {
        p.AddMember("scale", Config::physics.scale, doc->GetAllocator());
    }
    doc->AddMember("physics", p, doc->GetAllocator());
}

void deserializeGraphics(rapidjson::Value& g) {
    setInt(Config::graphics.width, g, "width");
    setInt(Config::graphics.height, g, "height");
    setBool(Config::graphics.fullscreen, g, "fullscreen");
    setDouble(Config::graphics.fov, g, "fov");
    setBool(Config::graphics.vsync, g, "vsync");
    setBool(Config::graphics.borderless, g, "borderless");
    setDouble(Config::graphics.nearPlaneClipping, g, "nearPlaneClipping");
    setDouble(Config::graphics.renderDistance, g, "renderDistance");
    setInt(Config::graphics.antialiasing, g, "antialiasing");
    setBool(Config::graphics.bobbing, g, "bobbing");
    setInt(Config::graphics.modelDetail, g, "modelDetail");
    setInt(Config::graphics.textureDetail, g, "textureDetail");
    setDouble(Config::graphics.fog, g, "fog");
    setInt(Config::graphics.anisotropicFiltering, g, "anisotropicFiltering");
    setInt(Config::graphics.shadowmapResolution, g, "shadowmapResolution");
    setInt(Config::graphics.depthBufferPrecision, g, "depthBufferPrecission");
}

void deserializePhysics(rapidjson::Value& p) {
    setDouble(Config::physics.scale, p, "scale");
}

void Config::serialize(rapidjson::Document* doc) {
    doc->SetObject();
    serializeGraphics(doc);
    serializePhysics(doc);
}

void Config::deserialize(rapidjson::Document* doc) {
    if (doc->HasMember("graphics")) {
        deserializeGraphics((*doc)["graphics"]);
    }
    if (doc->HasMember("physics")) {
        deserializePhysics((*doc)["physics"]);
    }
}