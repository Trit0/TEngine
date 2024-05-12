import { writeFileSync } from "node:fs";
import { Scene } from "./interfaces/scene";
import { Transform } from "./interfaces/transform";

const scene: Scene = {
    entities: [

    ]
}

const MAX_ENTITIES = 5000;

for (let i = 0; i < MAX_ENTITIES; i++) {
    const scale = randomInt(1, 4);
    scene.entities.push({
        transform: new Transform({
            x: randomInt(-50, 150),
            y: randomInt(-50, 150),
            z: randomInt(-50, 150)
        }, {x: randomInt(0, 3), y: randomInt(0, 3), z: randomInt(0, 3)}, {
            x: scale,
            y: scale,
            z: scale
        }),
        model: "../models/cube.obj"
    });
}

const json = JSON.stringify(scene, null, 2);

writeFileSync("../scenes/sceneTS.json", json, "utf-8");

console.log("Scene generated!");

function randomInt(min: number, max: number) {
    return Math.floor(Math.random() * (max - min + 1) + min);
}