"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const node_fs_1 = require("node:fs");
const scene = {
    test: true
};
const json = JSON.stringify(scene, null, 2);
(0, node_fs_1.writeFileSync)("../scenes/sceneTS.json", json, "utf-8");
