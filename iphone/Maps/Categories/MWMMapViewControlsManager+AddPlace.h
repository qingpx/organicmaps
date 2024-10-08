#import <Foundation/Foundation.h>
#include "geometry/point2d.hpp"

NS_ASSUME_NONNULL_BEGIN

@interface MWMMapViewControlsManager (AddPlace)

- (void)addPlace:(BOOL)isBusiness position:(m2::PointD const *)optionalPosition;

@end

NS_ASSUME_NONNULL_END
