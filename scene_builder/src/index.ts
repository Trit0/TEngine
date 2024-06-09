import { writeFileSync } from "node:fs";
import { Scene } from "./interfaces/scene";
import { Transform } from "./interfaces/transform";
import {bunchOfCubes, planeOfCubes} from "./scene_builders";

const scene: Scene = {
    entities: [

    ]
}

export const MAX_ENTITIES = 5000;

planeOfCubes(scene);

writeFileSync("../scenes/sceneTS.json",  JSON.stringify(scene, null, 2), "utf-8");

console.log("Scene generated!");

export function randomInt(min: number, max: number) {
    return Math.floor(Math.random() * (max - min + 1) + min);
}