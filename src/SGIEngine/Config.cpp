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
LogicConfig Config::logic;
NetworkConfig Config::network;

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
        g.AddMember("resizable", Config::graphics.resizeable, doc->GetAllocator());
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
        g.AddMember("ssao", Config::graphics.ssao, doc->GetAllocator());
    }
    doc->AddMember("graphics", g, doc->GetAllocator());
}

void serializePhysics(rapidjson::Document* doc) {
    rapidjson::Value p(rapidjson::kObjectType);
    {
        p.AddMember("scale", Config::physics.scale, doc->GetAllocator());
        p.AddMember("gravity", Config::physics.gravity, doc->GetAllocator());
        p.AddMember("correctionThreshold", Config::physics.correctionThreshold, doc->GetAllocator());
        p.AddMember("correctionPercentage", Config::physics.correctionPercentage, doc->GetAllocator());
    }
    doc->AddMember("physics", p, doc->GetAllocator());
}

void serializeLogic(rapidjson::Document* doc){
    rapidjson::Value l(rapidjson::kObjectType);
    {
        l.AddMember("updatesPerSecond", Config::logic.updatesPerSecond, doc->GetAllocator());
    }
    doc->AddMember("logic", l, doc->GetAllocator());
}

void serializeNetwork(rapidjson::Document* doc){
    rapidjson::Value n(rapidjson::kObjectType);
    {
        n.AddMember("port", Config::network.port, doc->GetAllocator());
        n.AddMember("ticksBetweenSnapshots", Config::network.ticksBetweenSnapshots, doc->GetAllocator());
    }
    doc->AddMember("network", n, doc->GetAllocator());
}

void deserializeGraphics(rapidjson::Value& g) {
    setInt(Config::graphics.width, g, "width");
    setInt(Config::graphics.height, g, "height");
    setBool(Config::graphics.fullscreen, g, "fullscreen");
    setDouble(Config::graphics.fov, g, "fov");
    setBool(Config::graphics.vsync, g, "vsync");
    setBool(Config::graphics.borderless, g, "borderless");
    setBool(Config::graphics.resizeable, g, "resizeable");
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
    setBool(Config::graphics.ssao, g, "ssao");
}

void deserializePhysics(rapidjson::Value& p) {
    setDouble(Config::physics.scale, p, "scale");
    setDouble(Config::physics.gravity, p, "gravity");
    setDouble(Config::physics.correctionPercentage, p, "correctionPercentage");
    setDouble(Config::physics.correctionThreshold, p, "correctionThreshold");
}

void deserializeLogic(rapidjson::Value& l) {
    setInt(Config::logic.updatesPerSecond, l, "updatesPerSecond");
}

void deserializeNetwork(rapidjson::Value& l) {
    setInt(Config::network.port, l, "port");
    setInt(Config::network.ticksBetweenSnapshots, l, "ticksBetweenSnapshots");
}

void Config::serialize(rapidjson::Document* doc) {
    doc->SetObject();
    serializeGraphics(doc);
    serializePhysics(doc);
    serializeLogic(doc);
    serializeNetwork(doc);
}

void Config::deserialize(rapidjson::Document* doc) {
    if (doc->HasMember("graphics")) {
        deserializeGraphics((*doc)["graphics"]);
    }
    if (doc->HasMember("physics")) {
        deserializePhysics((*doc)["physics"]);
    }
    if(doc->HasMember("logic")){
        deserializeLogic((*doc)["logic"]);
    }
    if(doc->HasMember("network")){
        deserializeNetwork((*doc)["network"]);
    }
}