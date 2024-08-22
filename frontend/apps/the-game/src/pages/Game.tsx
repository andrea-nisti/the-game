import { Box, Card } from '@chakra-ui/react';
import React from 'react';
// import { Player, TableCard } from '../components/GameComponents';
import { getRandomInt } from "../utils/Common"
import GameCard from '../components/GameCard';
import GameTable from '../components/Table';
import UserDeck from '../components/UserDeck';
import CompanionTab from '../components/CompanionTab';
import Player from '../components/Player';

interface GameProps {
  title: string;
}

interface GameState {
  userHand: GameCard[];
  companions: Player[];
}


class Game extends React.Component<GameProps, GameState> {
  constructor(props: GameProps) {
    super(props);
    this.state = {
      userHand: this.createInitialUserHand(6),

      companions: [
        new Player({ name: 'Prosepio Doroni', cardNumber: getRandomInt(1, 6), isUser: false }),
        new Player({ name: 'Prosepio dei Finacchioni', cardNumber: getRandomInt(1, 6), isUser: false }),
        new Player({ name: 'Madonnina Salutare', cardNumber: getRandomInt(1, 6), isUser: false })
      ],
    }
  }

  private createInitialUserHand(elements: number): GameCard[] {
    const newHand: GameCard[] = [];
    for (let i = 0; i < elements; i++) {
      newHand.push(new GameCard({ value: getRandomInt(1, 99), isActive: false, index: i }));
    }
    return newHand;
  }

  render() {
    return (
      <Box
        // sx={{ border: '10px solid red' }}
        display='flex'
        flexDirection='column-reverse'
        height='94vh'
        width='98.5vw'
        gap='8vh'>

        <UserDeck cards={this.state.userHand} />

        <GameTable />

        <CompanionTab players={this.state.companions} />

      </Box>
    );
  }
}

export default Game;