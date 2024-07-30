import { Box } from '@chakra-ui/react';
import React from 'react';
import { Player, TableCard } from '../components/GameComponents';
import {getRandomInt } from "../utils/Common"
import GameCard from '../components/GameCard';
import GameTable from '../components/Table';
import UserDeck from '../components/UserDeck';

interface GameProps {
  title: string;
}

interface GameState {
  userHand: GameCard[];
}


class Game extends React.Component<GameProps, GameState> {
  constructor(props: GameProps) {
    super(props);
    this.state = {
      userHand: [
        new GameCard({ value: getRandomInt(1, 99), isActive: false }),
        new GameCard({ value: getRandomInt(1, 99), isActive: false }),
        new GameCard({ value: getRandomInt(1, 99), isActive: false }),
        new GameCard({ value: getRandomInt(1, 99), isActive: false }),
        new GameCard({ value: getRandomInt(1, 99), isActive: false }),
        new GameCard({ value: getRandomInt(1, 99), isActive: false })
      ],
    };
  }

  render() {
    return (
      <Box
        // sx={{ border: '10px solid red' }}
        display='flex'
        flexDirection='column-reverse'
        height='94vh'
        width='98.5vw'
        gap='100px'>

        <UserDeck cards={this.state.userHand}/>
        <GameTable />

        <Box
          // sx={{ border: '10px solid white' }}
          justifyContent='center'
          alignItems='flex-end'
          display='flex'
          flexDirection='row'
          gap='50px'>
          <Player name="Prosepio Finacchioni" cardNumber={3} />
          <Player name="Marchionne Pampalone" cardNumber={3} />
          <Player name="Franco Sticchiano" cardNumber={3} />
        </Box>
      </Box>
    );
  }
}

export default Game;