# CHANGELOG

## [RELEASE]
### Added
- Implemented `isKingCheckmated` function to check if the current player's king is in check and if there are no valid moves left.
- Integrated checkmate detection into the `make_move` function to display a message when a player is checkmated.

### Changed
- **Move Validation Logic**:
  - Updated `is_valid_move` function to ensure that moves do not leave the player's king in check.
  - Temporarily makes the move to check if the king would be in check after the move.

- **Check Detection Logic**:
  - Updated `is_king_in_check` function to accurately check if the specified king is in check by iterating through the opponent's pieces.

- **Checkmate Detection Logic**:
  - Added logic in `isKingCheckmated` to determine if the current player's king is in check and if there are no valid moves left.

- **GUI Updates**:
  - Updated GUI logic to display a message indicating the winner when a player is checkmated.

## [Previous Versions]
- Initial release with basic chess game functionality.
