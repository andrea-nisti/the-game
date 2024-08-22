import React from "react";
import GameCard from "./GameCard";
import { Box } from "@chakra-ui/react";
import { getRandomInt } from "../utils/Common";

interface UserDeckProp {
    cards: GameCard[],
    activeIndex?: number,
    selectedCardValue?: number,
    debugBorder?: boolean
}

interface UserDeckState {
    cards: GameCard[],
    selectedCardValue?: number,
    activeIndex?: number,
    debugBorder?: boolean
}

export default class UserDeck extends React.Component<UserDeckProp, UserDeckState> {
    constructor(props: UserDeckProp) {
        super(props);

        this.state = {
            cards: props.cards,
            activeIndex: -1,
            selectedCardValue: props.selectedCardValue,
            debugBorder: props.debugBorder
        };
    }

    render() {
        return (

            <Box
                // sx={{ border: { '10px solid white' : '' } }}
                justifyContent='center'
                display='flex'
                flexDirection='row'
                gap='10px'
                marginBottom='100px'>

                {
                    this.state.cards.map((card: GameCard) => (

                        <GameCard
                            key={card.state.index}
                            isActive={card.state.index == this.state.activeIndex}
                            index={card.state.index}
                            value={card.state.value}
                            onClick={() => this.setState({ activeIndex: card.props.index })}
                        />
                    ))}
            </Box>
        )
    }

}
