// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    TArray<FString> Words;

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word) {
        return Word.Len() >= 4 && Word.Len() <=8 && IsIsogram(Word);
    });

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();

    if (bGameOver) {
        ClearScreen();
        SetupGame();
    } else {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame() {

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];

    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Hello there! Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("To do that, you have %i lives"), Lives);
    PrintLine(TEXT("Type in your guess and press enter to continue..."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    if (Guess == HiddenWord) {
        PrintLine(TEXT("You won !"));
        EndGame();
        return;
    } 
    
    if (!IsIsogram(Guess)) {
        PrintLine(TEXT("Is not an Isogram, guess again !"));
        return;
    }

    if (HiddenWord.Len() != Guess.Len()) {
        PrintLine(TEXT("The Hidden Word is %i characters longs, try again !"), HiddenWord.Len());
        return;
    }

    PrintLine(TEXT("You lost a life !")); 
    Lives--;

    if(Lives > 0) {
    } else {
        PrintLine(TEXT("You have no lives left !"));
        PrintLine(TEXT("You lose, play again ?"));
        EndGame();
        return;
    }
    FBullCowCount Score = GetBullsCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}


void UBullCowCartridge::EndGame() {
    bGameOver = true;
    PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
    PrintLine(TEXT("Press enter to play again."));
}

/* const TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words) {
    TArray<FString> ValidWords; 

    for (FString Word : Words)
    {
        if ((Word.Len() >= 4 && Word.Len() <=8) && IsIsogram(Word)) {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
} 
 */


bool UBullCowCartridge::IsIsogram(const FString& Word) {
    for(int32 Index = 0; Index < Word.Len(); Index++) {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++) {
            if(Word[Index] == Word[Comparison]) {
                return false;
            }
        }
    }
    
    return true;
}

const FBullCowCount UBullCowCartridge::GetBullsCows(const FString& Guess) {
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++) {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex]) {
            Count.Bulls ++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++) {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex]) {
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
    
}