import { Transform } from "./transform";

export interface Entity {
    transform: Transform;
    model?: string;
    texture?: string;
}