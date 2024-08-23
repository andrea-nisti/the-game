import GameCard from "../components/GameCard";
import Player from "../components/Player";
import TableCard from "../components/Table";
import { getRandomInt } from "./Common";

export function createInitialUserHand(elements: number): GameCard[] {
    const newHand: GameCard[] = [];
    for (let i = 0; i < elements; i++) {
        newHand.push(new GameCard({ value: getRandomInt(1, 99), isActive: false, index: i }));
    }
    return newHand;
}

export function createTableCards(elements: number): TableCard[] {
    const tableCards: TableCard[] = [];
    for (let i = 0; i < elements; i++) {
        tableCards.push();
    }
    return tableCards;
}


export function createCompanions(players: string[]): Player[] {
    const newHand: Player[] = [];
    for (let i = 0; i < players.length; i++) {
        newHand.push(new Player({ name: players[i], cardNumber: getRandomInt(1, 6), isUser: false}));
    }
    return newHand;
}