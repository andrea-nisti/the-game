import React from "react";
import { getRandomInt } from "../utils/Common";

interface GameCardProp {
    title: string;
}

interface GameCardState {
    value: number;
}

class GameCard extends React.Component<GameCardProp, GameCardState> {
    constructor(props: GameCardProp) {
        super(props);
        this.state = {
            value: getRandomInt(1, 99),
        };
    }
    render() {
        return (
            <div></div>
        )
    }
}

export default GameCard